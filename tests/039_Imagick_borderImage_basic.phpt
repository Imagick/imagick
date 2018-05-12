--TEST--
Test Imagick, borderImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$width = 50;
$height = 20;
$color = 'rgb(127, 127, 127)';

function borderImage($color, $width, $height) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->borderImage($color, $width, $height);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

borderImage($color, $width, $height) ;
echo "Ok";
?>
--EXPECT--
Ok