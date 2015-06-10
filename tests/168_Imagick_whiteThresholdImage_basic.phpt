--TEST--
Test Imagick, whiteThresholdImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$color = 'rgb(127, 127, 127)';

function whiteThresholdImage($color) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    //TODO needs a control
    $imagick->whiteThresholdImage($color);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

whiteThresholdImage($color) ;
echo "Ok";
?>
--EXPECTF--
Ok