--TEST--
Test ImagickDraw, matte
--SKIPIF--
<?php 
require_once(dirname(__FILE__) . '/skipif.inc'); 
checkClassMethods('ImagickDraw', array('matte'));
?>

--FILE--
<?php

$paintType = 4;
$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function matte($strokeColor, $fillColor, $backgroundColor, $paintType) {
    $draw = new \ImagickDraw();

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);

    $draw->setStrokeWidth(2);
    $draw->setFontSize(72);

    $draw->matte(120, 120, $paintType);
    
    $draw->rectangle(100, 100, 300, 200);
    

    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");
    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

matte($strokeColor, $fillColor, $backgroundColor, $paintType) ;
echo "Ok";
?>
--EXPECTF--
Ok