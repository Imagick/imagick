--TEST--
Test Imagick, levelizeImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('levelizeImage'));
?>
--FILE--
<?php


function levelizeImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $imagick->levelizeImage(0.1, 2.0, 0.95);
//    $imagick->writeImage(__DIR__ . '/claheImage_output_image.png');
    $imagick->getImageBlob();
}

levelizeImage();
echo "Ok";
?>
--EXPECTF--
Ok
