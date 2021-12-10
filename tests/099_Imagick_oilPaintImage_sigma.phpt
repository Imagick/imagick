--TEST--
Test Imagick, oilPaintImageWithSigma
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$radius = 5;

function oilPaintImage($radius) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->oilPaintImageWithSigma($radius, 2.0);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";}
}

oilPaintImage($radius) ;
echo "Ok";
?>
--EXPECTF--
Ok