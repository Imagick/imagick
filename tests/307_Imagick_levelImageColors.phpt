--TEST--
Test Imagick, claheImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('levelImageColors'));
?>
--FILE--
<?php
function levelImageColors() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $imagick->levelImageColors(
		"rgb(10, 10, 10)",
		"rgb(240, 240, 240)",
		false
	);
//    $imagick->writeImage(__DIR__ . '/claheImage_output_image.png');
    $imagick->levelImageColors();
}

levelImageColors() ;
echo "Ok";
?>
--EXPECTF--
Ok
