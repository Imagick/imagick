--TEST--
Test ImagickDraw, getDensity
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('ImagickDraw', array('getDensity', 'setDensity'));
?>
--XFAIL--
Expected behaviour is not known :-p
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

$draw = new \ImagickDraw();

$draw->setStrokeColor($strokeColor);
$draw->setFillColor($fillColor);

$draw->setStrokeWidth(2);
$draw->setFontSize(72);

$opacityToSet = 0.1;

$density = $draw->getDensity();

// What to test for density?

$draw->line(125, 70, 100, 50);
$draw->line(350, 170, 100, 150);

$imagick = new \Imagick();
$imagick->newImage(500, 500, $backgroundColor);
$imagick->setImageFormat("png");
$imagick->drawImage($draw);




$bytes = $imagick->getImageBlob();
if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "This test is not working yet.";
?>
--EXPECTF--
Ok