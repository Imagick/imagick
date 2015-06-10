--TEST--
Test Imagick, unsharpMaskImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$radius = 5;
$sigma = 1;
$amount = 5;
$unsharpThreshold = 0;
$channel = Imagick::CHANNEL_DEFAULT;

function unsharpMaskImage($radius, $sigma, $amount, $unsharpThreshold) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->unsharpMaskImage($radius, $sigma, $amount, $unsharpThreshold);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

unsharpMaskImage($radius, $sigma, $amount, $unsharpThreshold) ;
echo "Ok";
?>
--EXPECTF--
Ok