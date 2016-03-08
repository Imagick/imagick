--TEST--
Test ImagickDraw, getFontResolution
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('ImagickDraw', array('getFontResolution', 'setFontResolution'));
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

$fontResolution = $draw->getFontResolution();

if (isset($fontResolution["x"]) == false || isset($fontResolution["y"]) == false) {
	echo "$fontResolution doesn't contain expected values:\n";
	var_dump($fontResolution);
}

if ($fontResolution["x"] < 8 || $fontResolution["x"] > 100) {
	echo "Font resolution x outside expected range: ".$fontResolution["x"]."\n";
}
if ($fontResolution["y"] < 8 || $fontResolution["y"] > 100) {
	echo "Font resolution y outside expected range: ".$fontResolution["y"]."\n";
}

$resolutionToSet = 36;

$draw->setFontResolution($resolutionToSet, $resolutionToSet);
$fontResolution = $draw->getFontResolution();

if (abs($fontResolution["x"] - $resolutionToSet) > 0.0001) {
	echo "Font resolution x after set is not $resolutionToSet instead: ".$fontResolution["x"]."\n";
}
if (abs($fontResolution["y"] - $resolutionToSet) > 0.0001) {
	echo "Font resolution y after set is not $resolutionToSet instead: ".$fontResolution["y"]."\n";
}

$draw->line(125, 70, 100, 50);
$draw->annotation(50, 32, "Lorem Ipsum!");

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