--TEST--
Imagick::setImageAlpha
--SKIPIF--
<?php 
$imageMagickRequiredVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc'); 
?>
--FILE--
<?php

$imagick = new Imagick();
$imagick->newPseudoImage(256, 256, 'xc:purple');
$imagick->setImageAlpha(0.5);

$imagick->setImageFormat('png');
$imagick->writeImage("./setAlphaTest.png");

$pixelTypes = array(
	Imagick::PIXEL_CHAR => array(128, 0, 128, 128),
	Imagick::PIXEL_FLOAT => array(0.50196081399918, 0, 0.50196081399918, 0.5),
	Imagick::PIXEL_DOUBLE => array(0.50196078431373, 0, 0.50196078431373, 0.5),
	Imagick::PIXEL_SHORT => array(32896, 0, 32896, 32768),
);


foreach ($pixelTypes as $pixelType => $expectedValues) {
	$pixels = $imagick->exportImagePixels(0, 0, 1, 1, "RGBA", $pixelType);
	for ($i=0; $i<4; $i++) {
		$actual = $pixels[$i];
		$expected = $expectedValues[$i];
		if (abs($actual - $expected) > 0.0000001) {
			echo "Pixel values appear incorrect for pixelType $pixelType:";
			var_dump($pixels);
			break;
		}
	}
}

echo "Ok";

?>
--EXPECTF--
Ok