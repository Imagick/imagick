--TEST--
Test ImagickDraw, setStrokeDashOffset
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function setStrokeDashOffset($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(4);
    $draw->setStrokeDashArray(array(20, 20));
    $draw->setStrokeDashOffset(0);
    $draw->rectangle(100, 50, 225, 175);

    //Start the dash effect halfway through the solid portion
    $draw->setStrokeDashOffset(10);
    $draw->rectangle(275, 50, 400, 175);

    //Start the dash effect on the space portion
    $draw->setStrokeDashOffset(20);
    $draw->rectangle(100, 200, 225, 350);
    $draw->setStrokeDashOffset(5);
    $draw->rectangle(275, 200, 400, 350);

    $image = new \Imagick();
    $image->newImage(500, 400, $backgroundColor);
    $image->setImageFormat("png");
    $image->drawImage($draw);

    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setStrokeDashOffset($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok