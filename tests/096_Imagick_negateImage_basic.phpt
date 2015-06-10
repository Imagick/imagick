--TEST--
Test Imagick, negateImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$grayOnly = 0;
$channel = Imagick::CHANNEL_DEFAULT;

function negateImage($grayOnly, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->negateImage($grayOnly, $channel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

negateImage($grayOnly, $channel) ;
echo "Ok";
?>
--EXPECTF--
Ok