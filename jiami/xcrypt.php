<?
// Encrypt Function
function mc_encrypt($encrypt, $mc_key) {
    $iv = mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_256, MCRYPT_MODE_ECB), MCRYPT_RAND);
    $passcrypt = trim(mcrypt_encrypt(MCRYPT_RIJNDAEL_256, $mc_key, trim($encrypt), MCRYPT_MODE_ECB, $iv));
    $encode = base64_encode($passcrypt);
    return $encode;
}
 
// Decrypt Function
function mc_decrypt($decrypt, $mc_key) {
    $decoded = base64_decode($decrypt);
    $iv = mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_256, MCRYPT_MODE_ECB), MCRYPT_RAND);
    $decrypted = trim(mcrypt_decrypt(MCRYPT_RIJNDAEL_256, $mc_key, trim($decoded), MCRYPT_MODE_ECB, $iv));
    return $decrypted;
}


$res1 = mc_encrypt("helo", 'key');
$res2 = mc_decrypt($res1, 'key');
var_dump($res1, $res2);
?>
