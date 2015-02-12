--TEST--
Test Imagick, equalizeImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function equalizeImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->equalizeImage();
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

equalizeImage() ;
echo "Ok";
?>
--EXPECTF--
Ok