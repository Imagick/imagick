--TEST--
Test ImagickDraw, push
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';
$fillModifiedColor = 'LightCoral';

function push($strokeColor, $fillColor, $backgroundColor, $fillModifiedColor) {

    $draw = new \ImagickDraw();
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillModifiedColor);
    $draw->setStrokeWidth(2);
    $draw->setFontSize(72);
    $draw->push();
    $draw->translate(50, 50);
    $draw->rectangle(200, 200, 300, 300);
    $draw->pop();
    $draw->setFillColor($fillColor);
    $draw->rectangle(200, 200, 300, 300);

    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");

    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

push($strokeColor, $fillColor, $backgroundColor, $fillModifiedColor) ;
echo "Ok";
?>
--EXPECTF--
Ok