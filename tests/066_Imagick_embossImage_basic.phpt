--TEST--
Test Imagick, embossImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$radius = 5;
$sigma = 1;

function embossImage($radius, $sigma) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->embossImage($radius, $sigma);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

embossImage($radius, $sigma) ;
echo "Ok";
?>
--EXPECTF--
Ok