--TEST--
Test ImagickDraw, setStrokeAntialias
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function setStrokeAntialias($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(1);
    $draw->setStrokeAntialias(false);
    $draw->line(100, 100, 400, 105);

    $draw->line(100, 140, 400, 185);

    $draw->setStrokeAntialias(true);
    $draw->line(100, 110, 400, 115);
    $draw->line(100, 150, 400, 195);

    $image = new \Imagick();
    $image->newImage(500, 250, $backgroundColor);
    $image->setImageFormat("png");

    $image->drawImage($draw);

    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setStrokeAntialias($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok