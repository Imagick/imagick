--TEST--
Test Imagick, colorizeImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$color = 'rgb(127, 127, 127)';
$opacity = 100;

function colorizeImage($color, $opacity) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $opacity = $opacity / 255.0;
    $opacityColor = new \ImagickPixel("rgba(0, 0, 0, $opacity)");
    $imagick->colorizeImage($color, $opacityColor);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

colorizeImage($color, $opacity) ;
echo "Ok";
?>
--EXPECTF--
Ok