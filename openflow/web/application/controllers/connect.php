<?php

class Connect extends CI_Controller {

	/**
	 * Index Page for this controller.
	 *
	 * Maps to the following URL
	 * 		http://example.com/index.php/main
	 *	- or -
	 * 		http://example.com/index.php/main/index
	 *	- or -
	 * Since this controller is set as the default controller in
	 * config/routes.php, it's displayed at http://example.com/
	 *
	 * So any other public methods not prefixed with an underscore will
	 * map to /index.php/welcome/<method_name>
	 * @see http://codeigniter.com/user_guide/general/urls.html
	 */
	public function __construct()
	{
		parent::__construct();
	}

	public function index()
	{
		//echo "dwdwq";
		$this->load->library("MasterOpt");
		$result = $this->masteropt->submit_job(1);
		echo $result;
		$result = $this->masteropt->execute_jobinfo();
		echo $result;
		return;
		//$result = $this->masteropt->stop_job(1);
		//echo "<h1>".$result."</h1>";
		//$result = $this->masteropt->kill_job(2);
	}

	public function post()
	{
		
	}
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */
