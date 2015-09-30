--TEST--
Test Imagick, Imagick::exportImagePixels
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$imagick = new \Imagick();
$imagick->newPseudoImage(256, 256, "gradient:black-white");

$initialMask = $imagick->setImageChannelMask(\Imagick::CHANNEL_RED);
$imagick->brightnessContrastImage(-20, 20);
$imagick->setImageFormat("png");
$imagick->writeImage("./maskTest.png");

$redMask = $imagick->setImageChannelMask(\Imagick::CHANNEL_DEFAULT);

if ($initialMask != \Imagick::CHANNEL_DEFAULT) {
	echo "initialMask is not equal to \Imagick::CHANNEL_DEFAULT but instead is $initialMask\n";
}

if ($redMask != \Imagick::CHANNEL_RED) {
	echo "redMask is not equal to \Imagick::CHANNEL_RED but instead is $redMask\n";
}

echo "Ok";
?>
--EXPECTF--
Ok