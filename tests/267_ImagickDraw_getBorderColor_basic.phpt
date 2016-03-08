--TEST--
Test ImagickDraw, getBorderColor
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('ImagickDraw', array('getBorderColor', 'setBorderColor'));
?>
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

$borderColor = $draw->getBorderColor();
//var_dump($borderColor->getColor());

$draw->setBorderColor("red");
$borderColor = $draw->getBorderColor();

$borderColorElements = $borderColor->getColor();
if ($borderColorElements["r"] != 255 ||
	$borderColorElements["g"] != 0 ||
	$borderColorElements["b"] != 0) {
	echo "Wrong colors after set.";
}

$draw->line(125, 70, 100, 50);
$draw->line(350, 170, 100, 150);

$imagick = new \Imagick();
$imagick->newImage(500, 500, $backgroundColor);
$imagick->setImageFormat("png");
$imagick->drawImage($draw);




$bytes = $imagick->getImageBlob();
if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 



echo "Ok";
?>
--EXPECTF--
Ok