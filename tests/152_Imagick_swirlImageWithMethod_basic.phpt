--TEST--
Test Imagick, swirlImageWithMethod
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$swirl = 100;

function swirlImageWithMethod($swirl) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->swirlImageWithMethod($swirl, Imagick::INTERPOLATE_BILINEAR);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

swirlImageWithMethod($swirl) ;
echo "Ok";
?>
--EXPECTF--
Ok