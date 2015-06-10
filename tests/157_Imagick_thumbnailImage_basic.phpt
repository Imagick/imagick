--TEST--
Test Imagick, thumbnailImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function thumbnailImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->setbackgroundcolor('rgb(64, 64, 64)');
    $imagick->thumbnailImage(100, 100, true, true);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

thumbnailImage() ;
echo "Ok";
?>
--EXPECTF--
Ok