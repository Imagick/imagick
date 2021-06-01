--TEST--
Test Imagick, GetImageChannelRange basic
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--XFAIL--
Tests are not stable across ImageMagick versions.
--FILE--
<?php

$imagick = new \Imagick();
$imagick->newPseudoImage(640, 480, "gradient:rgb(0,32,32)-rgb(128,255,255)");

$redChannelRange = $imagick->getImageChannelRange(\Imagick::CHANNEL_RED);
$greenChannelRange = $imagick->getImageChannelRange(\Imagick::CHANNEL_GREEN);

$expectedRedMinima = (Imagick::getQuantum() * 0.0) / 255;
$expectedRedMaxima = (Imagick::getQuantum() * 128.0) / 255;

$expectedGreenMinima = (Imagick::getQuantum() * 32.0) / 255;
$expectedGreenMaxima = (Imagick::getQuantum() * 255.0) / 255;

// Floating point math. This is absolutely going to blow up
// on some platforms. But as finding out which would be interesting
// will leave it like this for now.
if ($expectedRedMinima !== $redChannelRange['minima']) {
	printf(
		"Unexpected red minima. Was expecting %s but have %s\n",
		$expectedRedMinima,
		$redChannelRange['minima']
	);
}
if ($expectedRedMaxima !== $redChannelRange['maxima']) {
	printf(
		"Unexpected red maxima. Was expecting %s but have %s\n",
		$expectedRedMinima,
		$redChannelRange['maxima']
	);
}


if ($expectedGreenMinima !== $greenChannelRange['minima']) {
	printf(
		"Unexpected green minima. Was expecting %s but have %s\n",
		$expectedGreenMinima,
		$greenChannelRange['minima']
	);
}
if ($expectedGreenMaxima !== $greenChannelRange['maxima']) {
	printf(
		"Unexpected green maxima. Was expecting %s but have %s\n",
		$expectedGreenMaxima,
		$greenChannelRange['maxima']
	);
}

echo "Ok";
?>
--EXPECTF--
Ok