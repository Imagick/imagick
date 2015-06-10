--TEST--
Test Imagick, linearStretchImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$blackThreshold = 0.2;
$whiteThreshold = 0.2;

function linearStretchImage($blackThreshold, $whiteThreshold) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $pixels = $imagick->getImageWidth() * $imagick->getImageHeight();
    $imagick->linearStretchImage($blackThreshold * $pixels, $whiteThreshold * $pixels);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

linearStretchImage($blackThreshold, $whiteThreshold) ;
echo "Ok";
?>
--EXPECTF--
Ok