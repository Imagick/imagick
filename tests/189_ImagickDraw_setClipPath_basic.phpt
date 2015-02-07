--TEST--
Test ImagickDraw, setClipPath
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function setClipPath($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeOpacity(1);
    $draw->setStrokeWidth(2);

    $clipPathName = 'testClipPath';

    $draw->pushClipPath($clipPathName);
    $draw->rectangle(0, 0, 250, 250);
    $draw->popClipPath();
    $draw->setClipPath($clipPathName);
    $draw->rectangle(100, 100, 400, 400);

    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");

    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setClipPath($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok