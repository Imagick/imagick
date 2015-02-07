--TEST--
Test ImagickDraw, arc
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
$startAngle = 0;
$endAngle = 270;

function arc($strokeColor, $fillColor, $backgroundColor, $startX, $startY, $endX, $endY, $startAngle, $endAngle) {

    //Create a ImagickDraw object to draw into.
    $draw = new \ImagickDraw();
    $draw->setStrokeWidth(1);
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(2);

    $draw->arc($startX, $startY, $endX, $endY, $startAngle, $endAngle);

    //Create an image object which the draw commands can be rendered into
    $image = new \Imagick();
    $image->newImage(500, 500, $backgroundColor);
    $image->setImageFormat("png");

    //Render the draw commands in the ImagickDraw object 
    //into the image.
    $image->drawImage($draw);

    //Send the image to the browser
    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

arc($strokeColor, $fillColor, $backgroundColor, $startX, $startY, $endX, $endY, $startAngle, $endAngle) ;
echo "Ok";
?>
--EXPECTF--
Ok