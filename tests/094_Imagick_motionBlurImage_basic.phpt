--TEST--
Test Imagick, motionBlurImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$radius = 20;
$sigma = 20;
$angle = 45;
$channel = Imagick::CHANNEL_DEFAULT;

function motionBlurImage($radius, $sigma, $angle, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->motionBlurImage($radius, $sigma, $angle, $channel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

motionBlurImage($radius, $sigma, $angle, $channel) ;
echo "Ok";
?>
--EXPECT--
Ok