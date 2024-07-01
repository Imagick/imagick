--TEST--
Test Imagick, setDepth
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('setDepth'));
?>
--FILE--
<?php

function setDepth() {
    $imagick = new \Imagick();

    $imagick->setDepth(16);
    $imagick->newPseudoImage(640, 480, "gradient:red-blue");

    echo "Depth is " .$imagick->getImageDepth() . "\n";

    // $imagick->getImageBlob();
    // TODO - check that the depth has actually worked?
    // this could be done by generating a gradient, and counting the unique
    // numbers of colours, or by looking at gradient artifacts.
}

setDepth() ;
echo "Ok";
?>
--EXPECTF--
Depth is 16
Ok
