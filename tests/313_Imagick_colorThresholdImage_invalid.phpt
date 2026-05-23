--TEST--
Test Imagick, colorThresholdImage invalid input
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('colorThresholdImage'));
?>
--FILE--
<?php

function colorThresholdImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    try {
        $imagick->colorThresholdImage(
            "rgb(10, 10, 10)",
            "complete garbage"
        );
    } catch (ImagickException $e) {
        echo $e::class, ": ", $e->getMessage(), "\n";
    }
}

colorThresholdImage() ;
?>
--EXPECTF--
ImagickException: Unrecognized color string
