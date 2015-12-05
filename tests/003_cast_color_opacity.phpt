--TEST--
Casting color and opacity to pixel
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php
print "--- Testing casts\n";

try {
	$im = new Imagick();
	$im->newImage(100, 100, "red");
	$im->tintImage("red", "gray(50%)");
	echo "Casting color and opacity succeeded\n";
} catch (Exception $e) {
	echo "Casting color and opacity failed: " , $e->getMessage() . PHP_EOL;
}

try {
	$im = new Imagick();
	$pixel = new ImagickPixel("red");
	$strengthPixel = new ImagickPixel("gray");
	$im->newImage(100, 100, $pixel);
	$im->tintImage($pixel, $strengthPixel);
	echo "Setting color and opacity without cast succeeded\n";
} catch (Exception $e) {
	echo "Setting color and opacity without cast failed: " , $e->getMessage() . PHP_EOL;
}

?>
--EXPECTF--
--- Testing casts
Casting color and opacity succeeded
Setting color and opacity without cast succeeded