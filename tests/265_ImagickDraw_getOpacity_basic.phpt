--TEST--
Test ImagickDraw, getOpacity
--SKIPIF--
<?php 
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('ImagickDraw', array('setOpacity'));
requirePHP("5.5.0");
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

$draw->setOpacity($opacityToSet);
$opacity = $draw->getOpacity();

$delta = abs($opacity - $opacityToSet);
if ($delta > 0.0001) {
	echo "Failed to get correct opacity, it should be $opacityToSet but got $opacity, which is a delta of $delta\n";
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