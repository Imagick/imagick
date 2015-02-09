--TEST--
Test ImagickDraw, setStrokeDashArray
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function setStrokeDashArray($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(4);

    $draw->setStrokeDashArray(array(10, 10));
    $draw->rectangle(100, 50, 225, 175);

    $draw->setStrokeDashArray(array(20, 5, 20, 5, 5, 5,));
    $draw->rectangle(275, 50, 400, 175);

    $draw->setStrokeDashArray(array(20, 5, 20, 5, 5));
    $draw->rectangle(100, 200, 225, 350);

    $draw->setStrokeDashArray(array(1, 1, 1, 1, 2, 2, 3, 3, 5, 5, 8, 8, 13, 13, 21, 21, 34, 34, 55, 55, 89, 89, 144, 144, 233, 233, 377, 377, 610, 610, 987, 987, 1597, 1597, 2584, 2584, 4181, 4181,));

    $draw->rectangle(275, 200, 400, 350);

    $image = new \Imagick();
    $image->newImage(500, 400, $backgroundColor);
    $image->setImageFormat("png");
    $image->drawImage($draw);

    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setStrokeDashArray($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok