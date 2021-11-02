--TEST--
Test Imagick, claheImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('claheImage'));
?>
--FILE--
<?php


function claheImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $imagick->claheImage(
		10,
		10,
		8,
		2
	);
//    $imagick->writeImage(__DIR__ . '/claheImage_output_image.png');
    $imagick->getImageBlob();
}

claheImage() ;
echo "Ok";
?>
--EXPECTF--
Ok
