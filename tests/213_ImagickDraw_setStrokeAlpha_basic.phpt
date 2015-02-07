--TEST--
Test ImagickDraw, setStrokeAlpha
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function setStrokeAlpha($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(4);
    $draw->line(100, 100, 400, 145);
    $draw->rectangle(100, 200, 225, 350);
    $draw->setStrokeOpacity(0.1);
    $draw->line(100, 120, 400, 165);
    $draw->rectangle(275, 200, 400, 350);

    $image = new \Imagick();
    $image->newImage(500, 400, $backgroundColor);
    $image->setImageFormat("png");

    $image->drawImage($draw);

    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setStrokeAlpha($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok