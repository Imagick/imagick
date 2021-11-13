--TEST--
Test Imagick, setExtract
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('setExtract'));
?>
--FILE--
<?php

function setExtract() {
    $imagick = new \Imagick();
    $imagick->setExtract("300x200+0+0");

    $imagick->readImage(__DIR__ . '/Biter_500.jpg');


//    $data = $imagick->getImageBlob();
//
//    $new_imagick = new Imagick();
//    $new_imagick->readImageBlob($data);

    echo "Width is " . $imagick->getImageWidth() . "\n";
    echo "Height is " . $imagick->getImageHeight() . "\n";
}

setExtract() ;
echo "Ok";
?>
--EXPECTF--
Width is 300
Height is 200
Ok
