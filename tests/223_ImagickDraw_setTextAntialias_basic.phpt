--TEST--
Test ImagickDraw, setTextAntialias
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

function setTextAntialias($fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();
    $draw->setStrokeColor('none');
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(1);
    $draw->setFontSize(32);
    $draw->setTextAntialias(false);
    $draw->annotation(5, 30, "Lorem Ipsum!");
    $draw->setTextAntialias(true);
    $draw->annotation(5, 65, "Lorem Ipsum!");

    $imagick = new \Imagick();
    $imagick->newImage(220, 80, $backgroundColor);
    $imagick->setImageFormat("png");
    $imagick->drawImage($draw);

    //Scale the image so that people can see the aliasing.
    $imagick->scaleImage(220 * 6, 80 * 6);
    $imagick->cropImage(640, 480, 0, 0);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setTextAntialias($fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok