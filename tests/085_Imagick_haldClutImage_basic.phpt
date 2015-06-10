--TEST--
Test Imagick, haldClutImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function haldClutImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagickPalette = new \Imagick();
    $imagickPalette->newPseudoImage(640, 480, "magick:NETSCAPE");

    $imagickPalette->sepiatoneImage(55);
    $imagick->haldClutImage($imagickPalette);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

haldClutImage() ;
echo "Ok";
?>
--EXPECTF--
Ok