--TEST--
Test Imagick, adaptiveResizeImage
--SKIPIF--
<?php 
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc'); 
?>
--FILE--
<?php

$width = 200;
$height = 200;
$bestFit = 1;

function adaptiveResizeImage($width, $height, $bestFit) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->adaptiveResizeImage($width, $height, $bestFit);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

adaptiveResizeImage($width, $height, $bestFit) ;
echo "Ok";
?>
--EXPECTF--
Ok