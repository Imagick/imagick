--TEST--
Test Imagick, charcoalImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$radius = 5;
$sigma = 1;

function charcoalImage($radius, $sigma) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->charcoalImage($radius, $sigma);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

charcoalImage($radius, $sigma) ;
echo "Ok";
?>
--EXPECTF--
Ok