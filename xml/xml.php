<?php
ini_set("user_agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.)");
function libxml_xsd_error($xml = '', $xsd =''){
    $xsd = empty($xsd) ? './abc.xsd' : $xsd;
    if(!file_exists($xml) || !file_exists($xsd)){
        return false;
    }   
    libxml_use_internal_errors(true);
    $xmlModel = new DOMDocument();
    $xmlModel->load($xml);
    if (!$xmlModel->schemaValidate($xsd)) {
        $errors = libxml_get_errors();
        //Çå³þlibxmlÖÐµÄ´íÎó
        libxml_clear_errors();
        return $errors;
    }   
    return true;
}
$rfile = $argv[1];
file_put_contents("./xml.xml", file_get_contents($rfile));
$a = libxml_xsd_error('./xml.xml','./abc.xsd');
var_dump($a);
