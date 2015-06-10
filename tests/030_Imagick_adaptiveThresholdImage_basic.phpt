--TEST--
Test Imagick, adaptiveThresholdImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc'); 
?>
--FILE--
<?php

$width = 50;
$height = 20;
$adaptiveOffset = 0.125;

function adaptiveThresholdImage($width, $height, $adaptiveOffset) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $adaptiveOffsetQuantum = intval($adaptiveOffset * \Imagick::getQuantum());
    $imagick->adaptiveThresholdImage($width, $height, $adaptiveOffsetQuantum);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

adaptiveThresholdImage($width, $height, $adaptiveOffset) ;
echo "Ok";
?>
--EXPECTF--
Ok