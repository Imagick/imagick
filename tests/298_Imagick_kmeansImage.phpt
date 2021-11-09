--TEST--
Test Imagick, kmeansImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('kmeansImage'));
?>
--FILE--
<?php


function kmeansImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $imagick->kmeansImage(
		128,
		10,
		5
	);

    $imagick->writeImage(__DIR__ . '/kmeansImage_output_image.png');
//    $imagick->getImageBlob();
}

kmeansImage() ;
echo "Ok";
?>
--CLEAN--
<?php
@unlink(__DIR__ . '/kmeansImage_output_image.png');
?>
--EXPECTF--
Ok
