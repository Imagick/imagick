--TEST--
Test Imagick, swirlImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$swirl = 100;

function swirlImage($swirl) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->swirlImage($swirl);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

swirlImage($swirl) ;
echo "Ok";
?>
--EXPECTF--
Ok