--TEST--
Test Imagick, complexImages
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('complexImages'));
?>
--FILE--
<?php

function complexImages() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $multiply = new Imagick();
    $multiply->newPseudoImage($imagick->getImageWidth(), $imagick->getImageHeight(), "gradient:black-white");
    $imagick->addImage($multiply);

    $imagick->complexImages(Imagick::COMPLEX_OPERATOR_MULTIPLY);
//    $imagick->writeImage(__DIR__ . '/complexImages_output_image.png');
    $imagick->getImageBlob();
}

complexImages() ;
echo "Ok";
?>
--EXPECTF--
Ok
