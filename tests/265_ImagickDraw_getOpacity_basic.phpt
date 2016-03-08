--TEST--
Test ImagickDraw, getOpacity
--SKIPIF--
<?php 
require_once(dirname(__FILE__) . '/skipif.inc'); 
requirePHP("5.5.0");
?>
--XFAIL--
Issue with setOpacity reported upstream https://github.com/ImageMagick/ImageMagick/issues/147
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

if (abs($opacity - $opacityToSet) > 0.000001) {
	echo "Failed to get correct opacity, it should be $opacityToSet but got $opacity\n";
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