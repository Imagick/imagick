--TEST--
Test Imagick, autoLevelImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');

?>
--FILE--
<?php


function autoLevelImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->autoLevelImage();
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

autoLevelImage() ;
echo "Ok";
?>
--EXPECTF--
Ok