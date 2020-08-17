--TEST--
Test Imagick, houghLineImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function houghLineImage() {
    $path = realpath(__DIR__ . '/houghline_input_image.png');

    if ($path === false) {
        echo "Image is not readable.\n";
        exit(-1);
    }

    $imagick = new \Imagick();
    $imagick->readImage($path);
    $imagick->setbackgroundcolor('rgb(64, 64, 64)');
    $imagick->houghLineImage(20,40, 40);
    $imagick->writeImage(__DIR__ . '/houghline_output_image.png');
}

houghLineImage() ;
echo "Ok";
?>
--EXPECTF--
Ok
