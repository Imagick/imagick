--TEST--
Test Imagick, waveletDenoiseImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('waveletDenoiseImage'));
?>
--FILE--
<?php


function waveletDenoiseImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');

    $imagick->waveletDenoiseImage(5, 5);
    $imagick->writeImage(__DIR__ . '/waveletDenoiseImage_output_image.png');
//    $imagick->getImageBlob();
}

waveletDenoiseImage() ;
echo "Ok";
?>
--CLEAN--
<?php
@unlink(__DIR__ . '/waveletDenoiseImage_output_image.png');
?>
--EXPECTF--
Ok
