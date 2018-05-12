--TEST--
Test Imagick, spliceImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$startX = 50;
$startY = 50;
$width = 50;
$height = 50;

function spliceImage($startX, $startY, $width, $height) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->spliceImage($width, $height, $startX, $startY);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

spliceImage($startX, $startY, $width, $height) ;
echo "Ok";
?>
--EXPECT--
Ok