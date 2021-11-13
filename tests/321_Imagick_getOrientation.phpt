--TEST--
Test Imagick, getOrientation/setOrientation
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('getOrientation'));
?>
--FILE--
<?php

function getOrientation() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $orientation = $imagick->getOrientation();

    echo "Orientation is $orientation\n";
    $imagick->setOrientation(Imagick::ORIENTATION_LEFTBOTTOM);
	$orientation = $imagick->getOrientation();

    echo "Orientation is now $orientation\n";
}

getOrientation() ;
echo "Ok";
?>
--EXPECTF--
Orientation is 0
Orientation is now 8
Ok
