--TEST--
Test Imagick, brightnessContrastImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$brightness = -20;
$contrast = -20;
$channel = Imagick::CHANNEL_DEFAULT;

function brightnessContrastImage($brightness, $contrast, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->brightnessContrastImage($brightness, $contrast, $channel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

brightnessContrastImage($brightness, $contrast, $channel) ;
echo "Ok";
?>
--EXPECTF--
Ok