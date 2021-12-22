--TEST--
Test Imagick, polaroidImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x631;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function polaroidImage() {
    $src1 = new \Imagick();
    $src1->newPseudoImage(640, 480, "magick:logo");

    $imagickDraw = new \ImagickDraw();
    $src1->polaroidImage($imagickDraw, 15);

    $src1->setImageFormat('png');
    $bytes = $src1->getImagesBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

polaroidImage() ;
echo "Ok";
?>
--EXPECTF--
Ok