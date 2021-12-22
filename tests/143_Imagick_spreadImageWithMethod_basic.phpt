--TEST--
Test Imagick, spreadImageWithMethod
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$radius = 5;

function spreadImageWithMethod($radius) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->spreadImageWithMethod($radius, Imagick::INTERPOLATE_BILINEAR);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

spreadImageWithMethod($radius) ;
echo "Ok";
?>
--EXPECTF--
Ok