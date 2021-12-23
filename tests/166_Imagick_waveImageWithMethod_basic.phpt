--TEST--
Test Imagick, waveImageWithMethod
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$amplitude = 5;
$length = 20;

function waveImageWithMethod($amplitude, $length) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->waveImageWithMethod($amplitude, $length, Imagick::INTERPOLATE_BILINEAR);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

waveImageWithMethod($amplitude, $length) ;
echo "Ok";
?>
--EXPECTF--
Ok