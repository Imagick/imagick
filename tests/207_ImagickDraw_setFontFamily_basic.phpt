--TEST--
Test ImagickDraw, setFontFamily
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function setFontFamily($fillColor, $strokeColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $strokeColor = new \ImagickPixel($strokeColor);
    $fillColor = new \ImagickPixel($fillColor);

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    //TODO - actually use setFontFamily
    $draw->setStrokeWidth(2);

    $draw->setFontSize(24);
    $draw->annotation(50, 50, "Lorem Ipsum!");

    $draw->setFontSize(36);
    $draw->annotation(50, 100, "Lorem Ipsum!");

    $draw->setFontSize(48);
    $draw->annotation(50, 150, "Lorem Ipsum!");

    $draw->setFontSize(60);
    $draw->annotation(50, 200, "Lorem Ipsum!");

    $draw->setFontSize(72);
    $draw->annotation(50, 250, "Lorem Ipsum!");

    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");
    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setFontFamily($fillColor, $strokeColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok