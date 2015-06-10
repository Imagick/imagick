--TEST--
Test Imagick, vignetteImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$blackPoint = 10;
$whitePoint = 10;
$x = 10;
$y = 10;

function vignetteImage($blackPoint, $whitePoint, $x, $y) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->vignetteImage($blackPoint, $whitePoint, $x, $y);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

vignetteImage($blackPoint, $whitePoint, $x, $y) ;
echo "Ok";
?>
--EXPECTF--
Ok