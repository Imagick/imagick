--TEST--
Test ImagickDraw, rotate
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';
$fillModifiedColor = 'LightCoral';

function rotate($strokeColor, $fillColor, $backgroundColor, $fillModifiedColor) {
    $draw = new \ImagickDraw();
    $draw->setStrokeColor($strokeColor);
    $draw->setStrokeOpacity(1);
    $draw->setFillColor($fillColor);
    $draw->rectangle(200, 200, 300, 300);
    $draw->setFillColor($fillModifiedColor);
    $draw->rotate(15);
    $draw->rectangle(200, 200, 300, 300);

    $image = new \Imagick();
    $image->newImage(500, 500, $backgroundColor);
    $image->setImageFormat("png");
    $image->drawImage($draw);

    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

rotate($strokeColor, $fillColor, $backgroundColor, $fillModifiedColor) ;
echo "Ok";
?>
--EXPECTF--
Ok