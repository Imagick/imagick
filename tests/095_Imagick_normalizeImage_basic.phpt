--TEST--
Test Imagick, normalizeImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$channel = Imagick::CHANNEL_DEFAULT;

function normalizeImage($channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $original = clone $imagick;
    $original->cropimage($original->getImageWidth() / 2, $original->getImageHeight(), 0, 0);
    $imagick->normalizeImage($channel);
    $imagick->compositeimage($original, \Imagick::COMPOSITE_ATOP, 0, 0);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

normalizeImage($channel) ;
echo "Ok";
?>
--EXPECTF--
Ok