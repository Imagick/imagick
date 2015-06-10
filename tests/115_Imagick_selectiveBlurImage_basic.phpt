--TEST--
Test Imagick, selectiveBlurImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$radius = 5;
$sigma = 1;
$threshold = 0.5;
$channel = Imagick::CHANNEL_DEFAULT;

function selectiveBlurImage($radius, $sigma, $threshold, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->selectiveBlurImage($radius, $sigma, $threshold, $channel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

selectiveBlurImage($radius, $sigma, $threshold, $channel) ;
echo "Ok";
?>
--EXPECTF--
Ok