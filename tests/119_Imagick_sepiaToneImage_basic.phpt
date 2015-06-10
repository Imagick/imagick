--TEST--
Test Imagick, sepiaToneImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$sepia = 55;

function sepiaToneImage($sepia) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->sepiaToneImage($sepia);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

sepiaToneImage($sepia) ;
echo "Ok";
?>
--EXPECTF--
Ok