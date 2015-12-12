--TEST--
Test Imagick, scaleImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


$tests = array(
    array(200, 200, true, 200, 150),
    array(200, 200, false, 200, 200),
    array(200, 0, false, 200, 150),
);

foreach ($tests as $test) {
    list($width, $height, $bestFit, $expectedWidth, $expectedHeight) = $test;

    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->scaleImage($width, $height, $bestFit);

	$imageWidth = $imagick->getImageWidth();
	$imageHeight = $imagick->getImageHeight();

	$error = false;

	if ($imageWidth != $expectedWidth) {
		echo "Width $imageWidth does not match expected.\n";
		$error = true;
	}

	if ($imageHeight != $expectedHeight) {
		echo "Height $imageHeight does not match expected.\n";
		$error = true;
	}

	if ($error) {
		echo "test was ".var_export($test, true)."\n";
	};
}


echo "Ok";
?>
--EXPECTF--
Ok