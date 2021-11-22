--TEST--
Test Imagick, interpolativeResizeImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('interpolativeResizeImage'));
?>
--FILE--
<?php


function interpolativeResizeImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $imagick->interpolativeResizeImage(
		320,
		200,
		Imagick::INTERPOLATE_CATROM
	);
//    $imagick->writeImage(__DIR__ . '/claheImage_output_image.png');
    $imagick->getImageBlob();
}

interpolativeResizeImage() ;
echo "Ok";
?>
--EXPECTF--
Ok
