--TEST--
Test Imagick, randomThresholdImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$lowThreshold = 0.1;
$highThreshold = 0.9;
$channel = Imagick::CHANNEL_DEFAULT;

function randomThresholdimage($lowThreshold, $highThreshold, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

    $imagick->randomThresholdimage(
        $lowThreshold * \Imagick::getQuantum(),
        $highThreshold * \Imagick::getQuantum(),
        $channel
    );
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

randomThresholdimage($lowThreshold, $highThreshold, $channel) ;
echo "Ok";
?>
--EXPECTF--
Ok