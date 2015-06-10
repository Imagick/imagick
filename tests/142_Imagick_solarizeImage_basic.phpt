--TEST--
Test Imagick, solarizeImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$solarizeThreshold = 0.2;

function solarizeImage($solarizeThreshold) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->solarizeImage($solarizeThreshold * \Imagick::getQuantum());
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

solarizeImage($solarizeThreshold) ;
echo "Ok";
?>
--EXPECTF--
Ok