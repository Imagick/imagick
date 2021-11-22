--TEST--
Test Imagick, averageImages
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('averageImages'));
?>
--FILE--
<?php


function averageImages() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:LOGO");
    $imagick2 = new \Imagick();
    $imagick2->newPseudoImage(640, 480, "gradient:black-white");


    $imagick->addImage($imagick2);
    $imagick->setIteratorIndex(0);

    $result_imagick = $imagick->averageImages();
    $result_imagick->setImageFormat('png');
    $result_imagick->writeImage(__DIR__ . '/averageImages_output_image.png');
    $result_imagick->getImageBlob();
}

averageImages() ;
echo "Ok";
?>
--CLEAN--
<?php
@unlink(__DIR__ . '/averageImages_output_image.png');
?>
--EXPECTF--
Ok
