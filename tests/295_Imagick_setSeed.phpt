--TEST--
Test Imagick, setSeed
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('setSeed'));
?>
--FILE--
<?php


function setSeed() {

	Imagick::setSeed(50);

    $imagick = new \Imagick();
    $imagick->newPseudoImage(256, 256, "plasma:tomato-steelblue");

//    $imagick->writeImage(__DIR__ . '/cannyEdgeImage_output_image.png');
//    $imagick->getImageBlob();

	Imagick::setSeed(50);

    $imagick = new \Imagick();
    $imagick->newPseudoImage(256, 256, "plasma:tomato-steelblue");

    // TODO - compare images.
}

setSeed() ;
echo "Ok";
?>
--EXPECTF--
Ok
