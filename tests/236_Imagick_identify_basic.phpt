--TEST--
Test Imagick, identifyImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$imagick = new \Imagick();
$imagick->newPseudoImage(640, 480, "magick:logo");
$imagick->setImageFormat('png');
$data = $imagick->identifyimage();

echo "format: " . $data["format"] . "\n";
echo "units: " . $data["units"] . "\n";
echo "type: " . $data["type"] . "\n";

if (array_key_exists('geometry', $data)) {
	$geometry = $data['geometry'];
	if (array_key_exists('width', $geometry) && array_key_exists('height', $geometry)) {
		printf(
			"Image geometry %dx%d",
			$geometry['width'],
			$geometry['height']
		);
		exit(0);
	}
}

echo "Failed get geometry from identifyimage:\n";
var_dump($data);

?>
--EXPECTF--
format: PNG (Portable Network Graphics)
units: Undefined
type: Palette
Image geometry 640x480