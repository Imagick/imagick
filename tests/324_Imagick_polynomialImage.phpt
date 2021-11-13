--TEST--
Test Imagick, polynomialImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('polynomialImage'));
?>
--FILE--
<?php

function polynomialImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $imagick_gradient = new \Imagick();
    $imagick_gradient->newPseudoImage(
        $imagick->getImageWidth(),
        $imagick->getImageHeight(),
        "gradient:black-white"
    );

    $imagick->addImage($imagick_gradient);

    $terms = [1, 1, 0.1, 0.1];

    // is 2 x (number_terms + 1 (the constant).
    $imagick->polynomialImage($terms);
    $imagick->writeImage(__DIR__ . '/polynomialImage_output_image.png');
    $imagick->getImageBlob();
}

polynomialImage() ;
echo "Ok";
?>
--EXPECTF--
Ok
