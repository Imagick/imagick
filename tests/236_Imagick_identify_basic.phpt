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
$imagick->setFormat('png');
$data = $imagick->identifyimage();

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
Image geometry 640x480