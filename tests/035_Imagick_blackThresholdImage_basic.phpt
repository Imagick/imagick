--TEST--
Test Imagick, blackThresholdImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$thresholdColor = 'rgb(127, 127, 127)';

function blackThresholdImage($thresholdColor) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->blackthresholdimage($thresholdColor);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

blackThresholdImage($thresholdColor) ;
echo "Ok";
?>
--EXPECTF--
Ok