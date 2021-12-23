--TEST--
Test Imagick, addNoiseImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x702;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$noiseType = Imagick::NOISE_IMPULSE;
$channel = Imagick::CHANNEL_DEFAULT;

function addNoiseImage($noiseType, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->addNoiseImage($noiseType, $channel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";}
    echo "addNoiseImage\n";
}

function addNoiseWithAttenuate($noiseType, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->addNoiseImageWithAttenuate($noiseType, 1.4, $channel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";}
    echo "addNoiseWithAttenuate\n";
}

addNoiseImage($noiseType, $channel);
addNoiseWithAttenuate($noiseType, $channel);
echo "Ok";
?>
--EXPECTF--
addNoiseImage
addNoiseWithAttenuate
Ok