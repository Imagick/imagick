--TEST--
Test Imagick, blueShiftImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$blueShift = 1.5;

function blueShiftImage($blueShift) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->blueShiftImage($blueShift);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

blueShiftImage($blueShift) ;
echo "Ok";
?>
--EXPECTF--
Ok