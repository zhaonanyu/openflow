/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: jian yi, eyjian@qq.com
 */
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdexcept>
#include <glog/logging.h>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include "main_template.h"

namespace common {
/***
 * 是否自重启，下列信号发生时，进程是否自重启:
 * 1) SIGILL
 * 2) SIGFPE
 * 3) SIGBUS
 * 4) SIGABRT
 * 5) SIGSEGV
 */
static bool self_restart(IMainHelper* main_helper);

/***
 * 子进程处理逻辑
 */
static void child_process(IMainHelper* main_helper, int argc, char* argv[]);

/***
 * 父进程处理逻辑
 * @child_pid: 子进程号
 * @child_exit_code: 子进程的退出代码
 * @return: 返回true的情况下才会自重启，否则父子进程都退出
 */
static bool parent_process(IMainHelper* main_helper, pid_t child_pid, int& child_exit_code);

/***
 * main_template总是在main函数中调用，通常如下一行代码即可:
 * int main(int argc, char* argv[])
 * {
 * return main_template(argc, argv);
 * }
 */
int main_template(IMainHelper* main_helper, int argc, char* argv[])
{
    // 退出代码，由子进程决定
    int exit_code = 1;

    // 忽略掉PIPE信号
    if (main_helper->ignore_pipe_signal())
    {
        if (SIG_ERR == signal(SIGPIPE, SIG_IGN))
        {
            fprintf(stderr, "Ignored SIGPIPE error: %m.\n");
            return 1;
        }
    }

    while (true)
    {
        pid_t pid = self_restart(main_helper)? fork(): 0;
        if (-1 == pid)
        {
            // fork失败
            fprintf(stderr, "fork error: %m.\n");
            break;
        }
        else if (0 == pid)
        {
            child_process(main_helper, argc, argv);
        }
        else if (!parent_process(main_helper, pid, exit_code))
        {
            break;
        }
    }

    return exit_code;
}

bool self_restart(IMainHelper* main_helper)
{
    std::string env_name = main_helper->get_restart_env_name();
    boost::trim(env_name);

    // 如果环境变量名为空，则认为不自重启
    if (env_name.empty()) return false;

    // 由环境变量SELF_RESTART来决定是否自重启
    char* restart = getenv(env_name.c_str());
    return (restart != NULL)
        && (0 == strcasecmp(restart, "true"));
}

void child_process(IMainHelper* main_helper, int argc, char* argv[])
{
    int errcode = 0;
    sigset_t sigset;
    sigset_t old_sigset;

    int exit_signo = main_helper->get_exit_signal();
    if (exit_signo > 0)
    {
        // 收到SIGUSR1信号时，则退出进程
        if (-1 == sigemptyset(&sigset))
        {
            fprintf(stderr, "Initialized signal set error: %m.\n");
            exit(1);
        }
        if (-1 == sigaddset(&sigset, exit_signo))
        {
            fprintf(stderr, "Added %s to signal set error: %m.\n", strsignal(exit_signo));
            exit(1);
        }
        if (-1 == sigprocmask(SIG_BLOCK, &sigset, &old_sigset))
        {
            fprintf(stderr, "Blocked SIGUSR1 error: %m\n");
            exit(1);
        }
    }

    // 请注意：只有在init成功后，才可以使用__MYLOG_INFO写日志，否则这个时候日志器可能还未created出来
    if (!main_helper->init(argc, argv))
    {
        //fprintf(stderr, "Main helper initialized failed.\n");
        main_helper->fini();
        exit(1);
    }
    if (!main_helper->run())
    {
        //fprintf(stderr, "Main helper run failed.\n");
        main_helper->fini();
        exit(1);
    }

    // 记录用来退出的信号
    //__LOG_INFO(main_helper->get_logger(), "Exit signal is %s .\n", strsignal(exit_signo));

    // 记录工作进程号
    //__LOG_INFO(main_helper->get_logger(), "Work process is %d.\n", sys::CUtil::get_current_process_id());

    while (exit_signo > 0)
    {
        int signo = -1;
        errcode = sigwait(&sigset, &signo);
        if (EINTR == errcode)
        {
            continue;
        }
        if (errcode != 0)
        {
            LOG(ERROR) << "Waited signal error: " << strerror(errno);
            break;
        }
        if (exit_signo == signo)
        {
            LOG(ERROR) << "Received exit signal " << strsignal(signo) << " and exited";
            break;
        }
    }

    main_helper->fini();
    exit(errcode);
}

bool parent_process(IMainHelper* main_helper, pid_t child_pid, int& child_exit_code)
{
    // 是否重启动
    bool restart = false;
    fprintf(stdout, "Parent process is %d, and its work process is %d.\n"
            , getpid(), child_pid);

    while (true)
    {
        int status;
        int retval = waitpid(child_pid, &status, 0);
        if (-1 == retval)
        {
            if (EINTR == errno)
            {
                continue;
            }
            else
            {
                fprintf(stderr, "Wait %d error: %m.\n", child_pid);
            }
        }
        else if (WIFSTOPPED(status))
        {
            child_exit_code = WSTOPSIG(status);
            fprintf(stderr, "Process %d was stopped by signal %d.\n"
                    , child_pid, child_exit_code);
        }
        else if (WIFEXITED(status))
        {
            child_exit_code = WEXITSTATUS(status);
            fprintf(stderr, "Process %d was exited with code %d.\n"
                    , child_pid, child_exit_code);
        }
        else if (WIFSIGNALED(status))
        {
            int signo = WTERMSIG(status);
            fprintf(stderr, "Process %d received signal %s.\n"
                    , child_pid, strsignal(signo));
            child_exit_code = signo;

            if ((SIGILL == signo) // 非法指令
                || (SIGBUS == signo) // 总线错误
                || (SIGFPE == signo) // 浮点错误
                || (SIGSEGV == signo) // 段错误
                || (SIGABRT == signo)) // raise
            {
                restart = true;
                fprintf(stderr, "Process %d will restart self for signal %s.\n"
                        , child_pid, strsignal(signo));

                // 延迟一秒，避免极端情况下拉起即coredump带来的死循环问题
                sleep(1);
            }
        }
        else
        {
            fprintf(stderr, "Process %d was exited, but unknown error.\n", child_pid);
        }

        break;
    }

    return restart;
}

} // end namespace common
