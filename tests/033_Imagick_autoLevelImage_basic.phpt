--TEST--
Test Imagick, autoLevelImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
$imageMagickRequiredVersion=675;
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