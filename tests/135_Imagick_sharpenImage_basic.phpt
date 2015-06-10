--TEST--
Test Imagick, sharpenImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$radius = 5;
$sigma = 1;
$channel = Imagick::CHANNEL_DEFAULT;

function sharpenImage($radius, $sigma, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->sharpenimage($radius, $sigma, $channel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

sharpenImage($radius, $sigma, $channel) ;
echo "Ok";
?>
--EXPECTF--
Ok