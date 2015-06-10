--TEST--
Test Imagick, oilPaintImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$radius = 5;

function oilPaintImage($radius) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->oilPaintImage($radius);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

oilPaintImage($radius) ;
echo "Ok";
?>
--EXPECTF--
Ok