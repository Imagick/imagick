--TEST--
Test Imagick, getImageRange
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('getImageRange'));
?>
--FILE--
<?php

require_once(__DIR__ . '/functions.inc');

function getImageRange() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(320, 240, "gradient:red-blue");
    $values = $imagick->getImageRange();

    check_value($values, "minima", 0);
    check_value($values, "maxima", 65535.0);
}

getImageRange() ;
echo "Ok";
?>
--EXPECTF--
Value for 'minima' is %f which is close enough to expected %f
Value for 'maxima' is %f which is close enough to expected %f
Ok
