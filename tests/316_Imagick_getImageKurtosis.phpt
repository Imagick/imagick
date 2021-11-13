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

    check_value($values, "kurtosis", -0.9379261035010518);
    check_value($values, "skewness", 0.4562517200972045);
}

getImageKurtosis() ;
echo "Ok";
?>
--EXPECTF--
Value for 'kurtosis' is %f which is close enough to expected %f
Value for 'skewness' is %f which is close enough to expected %f
Ok