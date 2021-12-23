--TEST--
Test Imagick, polaroidWithTextAndMethod
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

require_once(dirname(__FILE__) . '/functions.inc');

function polaroidWithTextAndMethod() {
    $src1 = new \Imagick();
    $src1->newPseudoImage(640, 480, "magick:logo");

    $imagickDraw = new \ImagickDraw();
    $font = findDefaultFont();
    $imagickDraw->setFont($font);

    $src1->polaroidWithTextAndMethod(
        $imagickDraw,
        15,
        "Hello world!",
        Imagick::INTERPOLATE_SPLINE
    );

    $src1->setImageFormat('png');
    $bytes = $src1->getImagesBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

polaroidWithTextAndMethod() ;
echo "Ok";
?>
--EXPECTF--
Ok