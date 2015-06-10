--TEST--
Test ImagickDraw, setFontStyle
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

function setFontStyle($fillColor, $strokeColor, $backgroundColor) {
    $draw = new \ImagickDraw();
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(1);
    $draw->setFontSize(36);
    $draw->setFontStyle(\Imagick::STYLE_NORMAL);
    $draw->annotation(50, 50, "Lorem Ipsum!");

    $draw->setFontStyle(\Imagick::STYLE_ITALIC);
    $draw->annotation(50, 100, "Lorem Ipsum!");

    $draw->setFontStyle(\Imagick::STYLE_OBLIQUE);
    $draw->annotation(50, 150, "Lorem Ipsum!");

    $imagick = new \Imagick();
    $imagick->newImage(350, 300, $backgroundColor);
    $imagick->setImageFormat("png");
    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setFontStyle($fillColor, $strokeColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok