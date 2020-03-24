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
	$imagick = new \Imagick();
        $imagick->readImage('houghline_input_image.png');
        $imagick->setbackgroundcolor('rgb(64, 64, 64)');
        $imagick->houghLineImage(20,40, 40);
        $imagick->writeImage('houghline_output_image.png');

}

houghLineImage() ;
echo "Ok";
?>
--EXPECTF--
Ok
