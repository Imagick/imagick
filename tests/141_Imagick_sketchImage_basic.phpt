--TEST--
Test Imagick, sketchImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$radius = 5;
$sigma = 1;
$angle = 45;

function sketchImage($radius, $sigma, $angle) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->sketchimage($radius, $sigma, $angle);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

sketchImage($radius, $sigma, $angle) ;
echo "Ok";
?>
--EXPECTF--
Ok