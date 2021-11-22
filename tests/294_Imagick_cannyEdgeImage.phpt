--TEST--
Test Imagick, cannyEdgeImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('cannyEdgeImage'));
?>
--FILE--
<?php


function cannyEdgeImage() {
    $path = realpath(__DIR__ . '/Biter_500.jpg');

    if ($path === false) {
        echo "Image is not readable.\n";
        exit(-1);
    }

    $imagick = new \Imagick();
    $imagick->readImage($path);

    $imagick->cannyEdgeImage(10, 4, 0.1, 0.5);
//    $imagick->writeImage(__DIR__ . '/cannyEdgeImage_output_image.png');
    $imagick->getImageBlob();
}

cannyEdgeImage() ;
echo "Ok";
?>
--EXPECTF--
Ok
