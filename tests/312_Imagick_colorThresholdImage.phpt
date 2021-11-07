--TEST--
Test Imagick, colorThresholdImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('colorThresholdImage'));
?>
--FILE--
<?php

function colorThresholdImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $imagick->colorThresholdImage(
        "rgb(10, 10, 10)",
        "rgb(240, 240, 240)"
    );
//    $imagick->writeImage(__DIR__ . '/colorThresholdImage_output_image.png');
    $imagick->getImageBlob();
}

colorThresholdImage() ;
echo "Ok";
?>
--EXPECTF--
Ok
