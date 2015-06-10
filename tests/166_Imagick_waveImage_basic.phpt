--TEST--
Test Imagick, waveImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$amplitude = 5;
$length = 20;

function waveImage($amplitude, $length) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->waveImage($amplitude, $length);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

waveImage($amplitude, $length) ;
echo "Ok";
?>
--EXPECTF--
Ok