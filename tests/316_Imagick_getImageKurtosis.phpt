--TEST--
Test Imagick, getImageKurtosis
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('getImageKurtosis'));
?>
--FILE--
<?php

require_once(__DIR__ . '/functions.inc');

function getImageKurtosis() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $values = $imagick->getImageKurtosis();

    check_value_posibilities(
        $values,
        "kurtosis",
        [-0.9379261035010518, -0.70925995674921]
    );
    check_value_posibilities(
        $values,
        "skewness",
        [0.4562517200972045, 0.56839010636614]
    );
}

getImageKurtosis() ;
echo "Ok";
?>
--EXPECTF--
Value for 'kurtosis' is %f which is close enough to expected %f
Value for 'skewness' is %f which is close enough to expected %f
Ok