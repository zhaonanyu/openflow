<!doctype html>
<html>
<head>
	<title>OpenFlow</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=7,IE=8,IE=9" />
	<meta name="viewport" content="initial-scale=1,maximum-scale=1,user-scalable=no" />
	<link rel="stylesheet" type="text/css" href="../content/css/admin/bootstrap.css" />
	<link rel="stylesheet" type="text/css" href="../content/css/admin/bootstrap-responsive.css" />
	<link rel="stylesheet" type="text/css" href="../content/css/admin/style.css" />
    // flows
    <link rel="stylesheet" type="text/css" href="../content/easyui/themes/default/easyui.css">
    <link rel="stylesheet" type="text/css" href="../content/easyui/themes/icon.css">
    <script src="../content/js/images.js"></script>

    <link href="../content/css/base.css?ver=<%= version%>" rel="stylesheet" />
    <link href="../content/jm/css/jm.paging.css?ver=<%= version%>" rel="stylesheet" />
    <link href="../content/jm/css/jm.table.css?ver=<%= version%>" rel="stylesheet" />
    <link href="../content/jm/css/jm.win.css?ver=<%= version%>" rel="stylesheet" />
    <link href="../content/jm/css/jm.codeHighLight.css?ver=<%= version%>" rel="stylesheet" />

	<script type="text/javascript" src="../content/js/jquery.min.js"></script>
	<script type='text/javascript' src='../content/js/admin/jsapi.js'></script>
	<script type="text/javascript" src="../content/js/admin/main.js"></script>
	<script type="text/javascript" src="../content/js/admin/bootstrap.min.js"></script>

    // flows
    <script src="../content/jm/js/jm.core.js"></script>
    <script src="../content/jm/js/jm.paging.js?ver=<%= version%>"></script>
    <script src="../content/jm/js/jm.table.js?ver=<%= version%>"></script>
    <script src="../content/jm/js/jm.template.js?ver=<%= version%>"></script>
    <script src="../content/jm/js/jm.codeHighLight.js?ver=<%= version%>"></script>
    <script src="../content/jm/js/jm.win.js?ver=<%= version%>"></script>

    <script src="../content/js/openflow.js?ver=<%= version%>"></script>
    <script src="../content/js/openflow.message.js?ver=<%= version%>"></script>

    <link href="../content/css/editor.css?ver=<%= version%>" rel="stylesheet" />
    <script type="text/javascript" src="../content/easyui/jquery.easyui.min.js"></script>

   <script src="../content/js/openflow.shapes.js?ver=<%= version%>"></script>
   <link href="../content/codemirror/lib/codemirror.css" rel="stylesheet" />
  <script src="../content/codemirror/lib/codemirror.js"></script>
    <script src="../content/codemirror/mode/shell/shell.js"></script>
    <script src="../content/codemirror/lib/util/runmode.js"></script>

<!-- jmgraph引用 -->
    <script type="text/javascript" src="../content/jmgraph/loadJMGraph.js" ></script>
<!-- jmgraph引用结束 -->

    <script src="../content/js/openflow.editor.js?ver=<%= version%>"></script>
    <script src="../content/js/openflow.editor.toolbar.js?ver=<%= version%>"></script>
    <script src="../content/js/openflow.code.js"></script>
    <script src="../content/js/openflow.category.js"></script>
    <script src="../content/js/openflow.editor.property.js?ver=<%= version%>"></script>

    <script src="../content/js/openflow.instance.js?ver=<%= version%>"></script>
    <script src="../content/js/openflow.base.js?ver=<%= version%>"></script>
    <script src="../content/js/openflow.executor.js?ver=<%= version%>"></script>
    <script src="../content/js/openflow.editor.page.js?ver=<%= version%>"></script>

    <script type="text/javascript">
        openflow.root = '../';
        openflow.user = 'helightxu';
        openflow.admin = 'true' == 'true';
        openflow.params = openflow.getParams();
        openflow.flag = <?php echo $mode; ?>; //openflow.params["mode"];//=1表示实例化,2表示查看,4=组件开发
        openflow.version = '<%= version%>';
        openflow.editable = openflow.flag == 0 || openflow.flag == 4;
    </script>

    <style type="text/css">
    .CodeMirror {
        height:auto;
        background-color: #fff;
        min-height: 150px;
        height: 98%;

        overflow: hidden;
    }
    .CodeMirror pre {

    }
    .codeview .CodeMirror  {
        border:0;
        background-color: transparent;
    }
    </style>
</head>
<?php
    require_once("list.php");
?>
