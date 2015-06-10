--TEST--
Test Imagick, addNoiseImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$noiseType = 2;
$channel = Imagick::CHANNEL_DEFAULT;

function addNoiseImage($noiseType, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->addNoiseImage($noiseType, $channel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

addNoiseImage($noiseType, $channel) ;
echo "Ok";
?>
--EXPECTF--
Ok