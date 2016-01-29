--TEST--
Test Imagick, Imagick::evaluateImages
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x687;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$imagick = new \Imagick();
$imagick->newPseudoImage(256, 256, "gradient:black-white");

$imagick2 = new \Imagick();
$imagick2->newPseudoImage(256, 256, "gradient:red-white");

$imagick->addImage($imagick2);
$imagick->setImageFormat('png');

$imagick->setIteratorIndex(0);

$evaluated = $imagick->evaluateImages(\Imagick::EVALUATE_MEAN);

$evaluated->setImageFormat('png');
// $evaluated->writeImage("./evaluateTest.png");
$data = $evaluated->getImageBlob();

if (strlen($data) < 50) {
	echo "Image data seems too short.";
}

echo "Ok";
?>
--EXPECTF--
Ok