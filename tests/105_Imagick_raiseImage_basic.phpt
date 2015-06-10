--TEST--
Test Imagick, raiseImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$width = 15;
$height = 15;
$x = 10;
$y = 10;
$raise = 1;

function raiseImage($width, $height, $x, $y, $raise) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

    //x and y do nothing?
    $imagick->raiseImage($width, $height, $x, $y, $raise);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

raiseImage($width, $height, $x, $y, $raise) ;
echo "Ok";
?>
--EXPECTF--
Ok