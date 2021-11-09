--TEST--
Test Imagick, meanShiftImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('meanShiftImage'));
?>
--FILE--
<?php


function meanShiftImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $imagick->meanShiftImage(
		10,
		10,
		5
	);

    $imagick->writeImage(__DIR__ . '/meanShiftImage_output_image.png');
//    $imagick->getImageBlob();
}

meanShiftImage() ;
echo "Ok";
?>
--CLEAN--
<?php
@unlink(__DIR__ . '/meanShiftImage_output_image.png');
?>
--EXPECTF--
Ok
