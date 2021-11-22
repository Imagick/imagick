--TEST--
Test Imagick, bilateralBlurImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('bilateralBlurImage'));
?>
--FILE--
<?php


function bilateralBlurImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');

    $imagick->bilateralBlurImage(
		5,
		1,
		2,
		2
	);

    $imagick->writeImage(__DIR__ . '/bilateralBlurImage_output_image.png');
//    $imagick->getImageBlob();
}

bilateralBlurImage() ;
echo "Ok";
?>
--CLEAN--
<?php
@unlink(__DIR__ . '/bilateralBlurImage_output_image.png');
?>
--EXPECTF--
Ok
