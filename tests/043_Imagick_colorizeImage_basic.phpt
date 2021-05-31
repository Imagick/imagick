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

$imagick = new \Imagick();
$imagick->newPseudoImage(640, 480, "magick:logo");
$opacity = $opacity / 255.0;
$opacityColor = new \ImagickPixel("rgba(0, 0, 0, $opacity)");
// TODO - legacy mode of setting fraction as transparency needs
// to be removed.
$imagick->colorizeImage($color, 0.5, true);
$bytes = $imagick->getImageBlob();
if (strlen($bytes) <= 0) { echo "Failed to generate image for legacy mode.";}

echo "Ok";
?>
--EXPECTF--
Ok