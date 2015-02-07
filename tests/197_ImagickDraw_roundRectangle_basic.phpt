--TEST--
Test ImagickDraw, roundRectangle
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';
$startX = 50;
$startY = 50;
$endX = 400;
$endY = 400;
$roundX = 100;
$roundY = 50;

function roundRectangle($strokeColor, $fillColor, $backgroundColor, $startX, $startY, $endX, $endY, $roundX, $roundY) {

    $draw = new \ImagickDraw();

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeOpacity(1);
    $draw->setStrokeWidth(2);

    $draw->roundRectangle($startX, $startY, $endX, $endY, $roundX, $roundY);

    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");

    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

roundRectangle($strokeColor, $fillColor, $backgroundColor, $startX, $startY, $endX, $endY, $roundX, $roundY) ;
echo "Ok";
?>
--EXPECTF--
Ok