--TEST--
Test Imagick, houghLineImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function houghLineImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->setbackgroundcolor('rgb(64, 64, 64)');
    $imagick->houghLinelImage(20,40, 40);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

houghLineImage() ;
echo "Ok";
?>
--EXPECTF--
Ok
