--TEST--
Test ImagickDraw, translate
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';
$fillModifiedColor = 'LightCoral';
$startX = 50;
$startY = 50;
$endX = 400;
$endY = 400;
$translateX = 75;
$translateY = 75;

function translate($strokeColor, $fillColor, $backgroundColor, $fillModifiedColor, 
                   $startX, $startY, $endX, $endY, $translateX, $translateY) {

    $draw = new \ImagickDraw();

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->rectangle($startX, $startY, $endX, $endY);

    $draw->setFillColor($fillModifiedColor);
    $draw->translate($translateX, $translateY);
    $draw->rectangle($startX, $startY, $endX, $endY);

    $image = new \Imagick();
    $image->newImage(500, 500, $backgroundColor);
    $image->setImageFormat("png");

    $image->drawImage($draw);

    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

translate($strokeColor, $fillColor, $backgroundColor, $fillModifiedColor,
    $startX, $startY, $endX, $endY, $translateX, $translateY);
echo "Ok";
?>
--EXPECTF--
Ok