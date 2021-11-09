--TEST--
Test Imagick, orderedDitherImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('orderedDitherImage'));
?>
--FILE--
<?php


function orderedDitherImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $imagick->orderedDitherImage("o3x3,6");
//    $imagick->writeImage(__DIR__ . '/claheImage_output_image.png');
    $imagick->getImageBlob();
}

orderedDitherImage() ;
echo "Ok";
?>
--EXPECTF--
Ok
