--TEST--
Test Imagick, chopImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$startX = 50;
$startY = 50;
$width = 100;
$height = 50;

function chopImage($startX, $startY, $width, $height) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->chopImage($width, $height, $startX, $startY);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

chopImage($startX, $startY, $width, $height) ;
echo "Ok";
?>
--EXPECTF--
Ok