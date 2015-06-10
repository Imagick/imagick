--TEST--
Test Imagick, annotateImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'rgb(232, 227, 232)';

function annotateImage($strokeColor, $fillColor) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

    $draw = new \ImagickDraw();
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);

    $draw->setStrokeWidth(1);
    $draw->setFontSize(36);
    
    $text = "Imagick is a native php \nextension to create and \nmodify images using the\nImageMagick API.";

    $imagick->annotateimage($draw, 40, 40, 0, $text);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

annotateImage($strokeColor, $fillColor) ;
echo "Ok";
?>
--EXPECTF--
Ok