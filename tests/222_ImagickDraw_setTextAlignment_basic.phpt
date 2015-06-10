--TEST--
Test ImagickDraw, setTextAlignment
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

function setTextAlignment($strokeColor, $fillColor, $backgroundColor) {
    $draw = new \ImagickDraw();
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(1);
    $draw->setFontSize(36);

    $draw->setTextAlignment(\Imagick::ALIGN_LEFT);
    $draw->annotation(250, 75, "Lorem Ipsum!");
    $draw->setTextAlignment(\Imagick::ALIGN_CENTER);
    $draw->annotation(250, 150, "Lorem Ipsum!");
    $draw->setTextAlignment(\Imagick::ALIGN_RIGHT);
    $draw->annotation(250, 225, "Lorem Ipsum!");
    $draw->line(250, 0, 250, 500);

    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");
    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setTextAlignment($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok