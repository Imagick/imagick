--TEST--
Test Imagick::importImagePixels() rejects dimensions that overflow instead of triggering undefined behaviour
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');

if (!method_exists("Imagick", "importImagePixels")) {
	die("skip Imagick::importImagePixels not available");
}

if (PHP_INT_SIZE < 8) {
	die("skip 64-bit platform required");
}
?>
--FILE--
<?php

$im = new Imagick();
$im->newImage(1, 1, 'gray');

// width*height overflow
try {
	$im->importImagePixels(0, 0, 4294967296, 4294967296, "RGB", Imagick::PIXEL_CHAR, array());
	echo "no exception thrown\n";
} catch (ImagickException $e) {
	echo $e->getMessage() . "\n";
}

// width*height*map overflow
try {
	$im->importImagePixels(0, 0, 4294967295, 4294967295, "RGB", Imagick::PIXEL_CHAR, array());
	echo "no exception thrown\n";
} catch (ImagickException $e) {
	echo $e->getMessage() . "\n";
}

// sane but incorrect count
try {
	$im->importImagePixels(0, 0, 2, 2, "RGB", Imagick::PIXEL_CHAR, array(0, 0, 0));
	echo "no exception thrown\n";
} catch (ImagickException $e) {
	echo $e->getMessage() . "\n";
}

echo "Ok\n";
?>
--EXPECTF--
The requested dimensions are too large to process
The requested dimensions are too large to process
The map contains incorrect number of elements. Expected 12, array has 3
Ok
