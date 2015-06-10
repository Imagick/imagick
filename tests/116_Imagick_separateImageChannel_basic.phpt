--TEST--
Test Imagick, separateImageChannel
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$channel = Imagick::CHANNEL_DEFAULT;

function separateImageChannel($channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->separateimagechannel($channel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

separateImageChannel($channel) ;
echo "Ok";
?>
--EXPECTF--
Ok