--TEST--
Test Imagick, getType
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('getType'));
?>
--FILE--
<?php

function test_getType() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $type = $imagick->getType();

    echo "Type is " . $type . "\n";
}

test_getType() ;
echo "Ok";
?>
--EXPECTF--
Type is 0
Ok
