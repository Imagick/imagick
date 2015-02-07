--TEST--
Test ImagickDraw, pathStart
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function pathStart($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeOpacity(1);
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);

    $draw->setStrokeWidth(2);
    $draw->setFontSize(72);

    $draw->pathStart();
    $draw->pathMoveToAbsolute(50, 50);
    $draw->pathLineToAbsolute(100, 50);
    $draw->pathLineToRelative(0, 50);
    $draw->pathLineToHorizontalRelative(-50);
    $draw->pathFinish();

    $draw->pathStart();
    $draw->pathMoveToAbsolute(50, 50);
    $draw->pathMoveToRelative(300, 0);
    $draw->pathLineToRelative(50, 0);
    $draw->pathLineToVerticalRelative(50);
    $draw->pathLineToHorizontalAbsolute(350);
    $draw->pathclose();
    $draw->pathFinish();

    $draw->pathStart();
    $draw->pathMoveToAbsolute(50, 300);
    $draw->pathCurveToAbsolute(50, 300, 100, 200, 300, 300);
    $draw->pathLineToVerticalAbsolute(350);
    $draw->pathFinish();

    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");

    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

pathStart($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok