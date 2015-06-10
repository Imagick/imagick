--TEST--
Test Imagick, rollImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$rollX = 100;
$rollY = 100;

function rollImage($rollX, $rollY) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->rollimage($rollX, $rollY);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

rollImage($rollX, $rollY) ;
echo "Ok";
?>
--EXPECTF--
Ok