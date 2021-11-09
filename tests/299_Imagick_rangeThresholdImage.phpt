--TEST--
Test Imagick, rangeThresholdImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('rangeThresholdImage'));
?>
--FILE--
<?php


function rangeThresholdImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');

    $imagick->rangeThresholdImage(
		32,
		64,
		192,
		228
	);

    $imagick->writeImage(__DIR__ . '/rangeThresholdImage_output_image.png');
//    $imagick->getImageBlob();
}

rangeThresholdImage() ;
echo "Ok";
?>
--CLEAN--
<?php
@unlink(__DIR__ . '/rangeThresholdImage_output_image.png');
?>
--EXPECTF--
Ok
