--TEST--
Imagick::getImageBlob behaviour on invalid images
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

// Fails due to image having no format
$imagick = new Imagick();
try {
    $imagick->newPseudoImage(200, 200, "xc:red");
	$result = $imagick->getImageBlob();
	echo "Imagick failed to throw exception" . PHP_EOL;
} catch (ImagickException $e) {
	echo "ImagickException: " . $e->getMessage() . PHP_EOL;
}

echo "Fin.\n";

?>
--EXPECTF--
ImagickException: Failed to get the image contents (empty or invalid image?)
Fin.