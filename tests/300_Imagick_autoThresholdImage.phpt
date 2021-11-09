--TEST--
Test Imagick, autoThresholdImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('autoThresholdImage'));
?>
--FILE--
<?php


function autoThresholdImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $imagick->autoThresholdImage(Imagick::AUTO_THRESHOLD_OTSU);
    $imagick->writeImage(__DIR__ . '/autoThresholdImage_output_image.png');
//    $imagick->getImageBlob();
}

autoThresholdImage() ;
echo "Ok";
?>
--CLEAN--
<?php
@unlink(__DIR__ . '/autoThresholdImage_output_image.png');
?>
--EXPECTF--
Ok
