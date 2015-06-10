--TEST--
Test Imagick, modulateImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$hue = 150;
$saturation = 100;
$brightness = 100;

function modulateImage($hue, $brightness, $saturation) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->modulateImage($brightness, $saturation, $hue);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

modulateImage($hue, $brightness, $saturation) ;
echo "Ok";
?>
--EXPECTF--
Ok