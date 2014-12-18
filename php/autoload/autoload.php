<?php
class AutoLoad {
	public function _autoload($class) {
		echo 'AutoLoad called', "\n";
		echo "class name: ", $class, "\n";
	}
}

/**
 * 动态
 */
$auto = new AutoLoad();
spl_autoload_register(array($auto, '_autoload'));

/**
 * AutoLoad::_autoload();
 */
//spl_autoload_register(array('AutoLoad', '_autoload'));
