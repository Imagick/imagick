--TEST--
Test ImagickDraw, getTextDirection
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('ImagickDraw', array('getTextDirection', 'setTextDirection'));
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
$draw->setFontSize(56);

$directions = array(
	\Imagick::DIRECTION_LEFT_TO_RIGHT,
	\Imagick::DIRECTION_RIGHT_TO_LEFT,
	\Imagick::DIRECTION_LEFT_TO_RIGHT,
);

$i = 0;
foreach ($directions as $directionToSet) {
	$draw->setTextDirection($directionToSet);
	$directionReturned = $draw->getTextDirection();

	if ($directionReturned != $directionToSet) {
		echo "Direction error for $i \n";
	}

	$position = 36;
	if ($directionToSet == \Imagick::DIRECTION_RIGHT_TO_LEFT) {
		$position = 500 - 36;
	}

	$draw->annotation($position, (72 * $i) + 64, "Lorem Ipsum!");

	$i++;
}

$imagick = new \Imagick();
$imagick->newImage(500, 500, $backgroundColor);
$imagick->setImageFormat("png");
$imagick->drawImage($draw);

$bytes = $imagick->getImageBlob();
if (strlen($bytes) <= 0) { echo "Failed to generate image.";}
 
$imagick->writeImage('./directionTest.png');

echo "Ok";
?>
--EXPECTF--
Ok