--TEST--
Test Imagick, resampleImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function resampleImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

    $imagick->resampleImage(200, 200, \Imagick::FILTER_LANCZOS, 1);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

resampleImage() ;
echo "Ok";
?>
--EXPECTF--
Ok