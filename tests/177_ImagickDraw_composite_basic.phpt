--TEST--
Test ImagickDraw, composite
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

function composite($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setFillOpacity(1);
    $draw->setStrokeWidth(2);
    $draw->setFontSize(72);
    $draw->setStrokeOpacity(1);
    $draw->setStrokeColor($strokeColor);
    $draw->setStrokeWidth(2);
    $draw->setFontSize(140);
    $draw->rectangle(0, 0, 1000, 300);
    $draw->setFillColor('white');
    $draw->setfillopacity(1);
    $draw->annotation(50, 180, "Lorem Ipsum!");

//    $imagick = new \Imagick(realpath("../images/TestImage.jpg"));
//    $draw->composite(\Imagick::COMPOSITE_MULTIPLY, -500, -200, 2000, 600, $imagick);

    //$imagick->compositeImage($draw, 0, 0, 1000, 500);
    //$draw->composite(Imagick::COMPOSITE_COLORBURN, -500, -200, 2000, 600, $imagick);

    //Create an image object which the draw commands can be rendered into
    $imagick = new \Imagick();
    $imagick->newImage(1000, 302, $backgroundColor);
    $imagick->setImageFormat("png");

    //Render the draw commands in the ImagickDraw object 
    //into the image.
    $imagick->drawImage($draw);

    //Send the image to the browser
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

composite($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok