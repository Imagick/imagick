--TEST--
Test Imagick, getInterpolateMethod/setInterpolateMethod
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('getInterpolateMethod'));
?>
--FILE--
<?php

function getInterpolateMethod() {
    $imagick = new \Imagick();


    $value = $imagick->getInterpolateMethod();
    echo "Interpolate method is " . $value . "\n";
    $imagick->newPseudoImage(640, 480, "gradient:red-blue");

    $imagick->setInterpolateMethod(Imagick::INTERPOLATE_BILINEAR);

    $value = $imagick->getInterpolateMethod();
    echo "Interpolate method is now " . $value . "\n";
}

getInterpolateMethod() ;
echo "Ok";
?>
--EXPECTF--
Interpolate method is 0
Interpolate method is now 5
Ok
