--TEST--
Test Imagick, whiteBalanceImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('whiteBalanceImage'));
?>
--FILE--
<?php

function whiteBalanceImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $imagick->whiteBalanceImage();
    $imagick->getImageBlob();
}

whiteBalanceImage() ;
echo "Ok";
?>
--EXPECTF--
Ok
