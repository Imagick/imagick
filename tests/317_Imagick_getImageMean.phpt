--TEST--
Test Imagick, getImageMean
--SKIPIF--
<?php
// mean calculation seems unstable on earlier versions
// lets only check modern versions, and see if it breaks
$imageMagickRequiredVersion=0x710;
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('getImageMean'));
?>
--FILE--
<?php

require_once(__DIR__ . '/functions.inc');

function getImageMean() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $values = $imagick->getImageMean();

    check_value_with_epsilon($values, "mean", 29267.518515000003, 0.2);
    check_value_with_epsilon($values, "standard_deviation", 18075.35838235082, 0.2);
}

getImageMean();
echo "Ok";
?>
--EXPECTF--
Value for 'mean' is %f which is close enough to expected %f
Value for 'standard_deviation' is %f which is close enough to expected %f
Ok
