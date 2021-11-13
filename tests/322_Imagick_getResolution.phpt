--TEST--
Test Imagick, getResolution
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('getResolution'));
?>
--FILE--
<?php

function getResolution() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $result = $imagick->getResolution();

    echo "x is " . $result['x'] . "\n";
    echo "y is " . $result['y'] . "\n";

    $imagick->setResolution(120, 60);
    $result = $imagick->getResolution();

    echo "x is now " . $result['x'] . "\n";
    echo "y is now " . $result['y'] . "\n";

    $imagick->getImageBlob();
}

getResolution() ;
echo "Ok";
?>
--EXPECTF--
x is 72
y is 72
x is now 120
y is now 60
Ok
