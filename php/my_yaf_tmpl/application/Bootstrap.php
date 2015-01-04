<?php
/**
 * @name Bootstrap
 * @author libofeng
 * @desc 所有在Bootstrap类中, 以_init开头的方法, 都会被Yaf调用,
 * @see http://www.php.net/manual/en/class.yaf-bootstrap-abstract.php
 * 这些方法, 都接受一个参数:Yaf_Dispatcher $dispatcher
 * 调用的次序, 和申明的次序相同
 */
class Bootstrap extends Yaf_Bootstrap_Abstract{

    public function _initConfig() {
		//把配置保存起来
		$arrConfig = Yaf_Application::app()->getConfig();
		Yaf_Registry::set('config', $arrConfig);
	}

	public function _initPlugin(Yaf_Dispatcher $dispatcher) {
		//注册一个插件
		$objSamplePlugin = new SamplePlugin();
		$dispatcher->registerPlugin($objSamplePlugin);
	}

	public function _initRoute(Yaf_Dispatcher $dispatcher) {
		//在这里注册自己的路由协议,默认使用简单路由
	}
	
	public function _initView(Yaf_Dispatcher $dispatcher){
		//在这里注册自己的view控制器，例如smarty,firekylin
		$smarty = new Smarty_Adapter(null, Yaf_Registry::get("config")->get("smarty"));
		$dispatcher->setView($smarty);
	}

	public function _initMemcache(Yaf_Dispatcher $dispatcher) {
		// 设置一致性哈希
		ini_set('memcache.hash_strategy', 'consistent');
		$cacheconfig = explode('|', $this->_config->application->memcacheConfig);
		$memcache = new Memcache;
		foreach ($cacheconfig as $cache) {
			list($host, $port) = explode(':', $cache);
			$memcache->addServer($host, $port, true, 1, null, 15, true, null, 300);
		}   

		$this->_memcache = $memcache;
		Yaf_Registry::set('memcache', $memcache);
	}

	public function _initMemcacheSession(){
		session_name('_S');
		session_set_save_handler(
			new MSessionHandler($this->_memcache, $this->_config->application->session_expire),
			true
		);
		session_set_cookie_params($this->_config->application->session_expire);
		session_start();
	}
}
