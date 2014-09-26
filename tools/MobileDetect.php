<?php
/**
 * MobileDetect 
 * 检测用户的UA
 * @package Lib
 * @time 2012-08-17
 * @author funkun liufuqiang 
 * */
class MobileDetect {

    protected $accept;
    protected $userAgent;
    protected $isMobile = false;
    protected $isAndroid = null;
    protected $isAndroidtablet = null;
    protected $isIphone = null;
    protected $isIpad = null;
    protected $isBlackberry = null;
    protected $isBlackberrytablet = null;
    protected $isOpera = null;
    protected $isPalm = null;
    protected $isWindows = null;
    protected $isWindowsphone = null;
    protected $isGeneric = null;
    protected $devices = array(
        "opera" => "opera (mini|mobi)",
        "android" => "android.*mobile",
        "androidtablet" => "android(?!.*mobile)",
        "blackberry" => "blackberry",
        "blackberrytablet" => "rim tablet os",
        "iphone" => "(iphone|ipod)",
        "ipad" => "(ipad)",
        "palm" => "(avantgo|blazer|elaine|hiptop|palm|plucker|xiino)",
        "windows" => "windows ce; (iemobile|ppc|smartphone)",
        "windowsphone" => "windows phone os",
        "generic" => "(acer|zte|lenovo|moto|samu|nokia|sony|kindle|240x320|mobile|mmp|ucweb|midp|pocket|psp|symbian|smartphone|treo|up.browser|up.link|vodafone|wap|opera mini)"
    );


    public function __construct()
    {
        $this->userAgent = $_SERVER['HTTP_USER_AGENT'];
        $this->accept = $_SERVER['HTTP_ACCEPT'];

        if (isset($_SERVER['HTTP_VIA']) && (stripos($_SERVER['HTTP_VIA'], 'wap') !== false)) {
            $this->isMobile = true;
        }elseif (isset($_SERVER['HTTP_X_WAP_PROFILE']) || isset($_SERVER['HTTP_PROFILE'])) {
            $this->isMobile = true;
        } elseif (stripos($this->accept, 'text/vnd.wap.wml') !== false || stripos($this->accept, 'application/vnd.wap.xhtml+xml') !== false) {
            $this->isMobile = true;
        } else {
            foreach ($this->devices as $device => $regexp) {
                if ($this->isDevice($device)) {
                        //ipad androidtable 走pc模式
                        if(in_array($device,array('ipad' , "androidtablet","blackberrytablet"))){
                            $this->isMobile = false;
                        } else{
                            $this->isMobile = true;
                        }
                        break;
                }
            }
        }
    }


    /**
     * Overloads isAndroid() | isAndroidtablet() | isIphone() | isIpad() | isBlackberry() | isBlackberrytablet() | isPalm() | isWindowsphone() | isWindows() | isGeneric() through isDevice()
     *
     * @param string $name
     * @param array $arguments
     * @return   bool
     */

    public function __call($name, $arguments)
    {
        $device = substr($name, 2);
        if ($name == "is" . ucfirst($device) && array_key_exists(strtolower($device), $this->devices)) {
            return $this->isDevice($device);
        } else {
            trigger_error("Method $name not defined", E_USER_WARNING);
        }
    }


    /**
     * Returns true if any type of mobile device detected, including special ones
     * @return   bool
     */
    public function isMobile()
    {
        return $this->isMobile;
    }

    protected function isDevice($device)
    {
        $var = "is" . ucfirst($device);
        $return = $this->$var === null ? (bool) preg_match("/" . $this->devices[strtolower($device)] . "/i", $this->userAgent) : $this->$var;
        if ($device != 'generic' && $return == true) {
            $this->isGeneric = false;
        }
        return $return;
    }
}
