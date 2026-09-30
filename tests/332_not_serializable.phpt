--TEST--
Imagick classes cannot be serialized
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

class MyImagick extends Imagick {}

foreach ([new Imagick(), new MyImagick(), new ImagickDraw(), new ImagickPixel()] as $object) {
	try {
		serialize($object);
		echo get_class($object) . " was serialized" . PHP_EOL;
	} catch (Exception $e) {
		echo $e->getMessage() . PHP_EOL;
	}
}

// Before PHP 8.1, only the C: format reaches the handler that throws
$format = PHP_VERSION_ID < 80100 ? 'C' : 'O';

$classes = ['Imagick', 'ImagickDraw', 'ImagickPixel', 'ImagickPixelIterator'];
if (class_exists('ImagickKernel')) {
	$classes[] = 'ImagickKernel';
}

foreach ($classes as $class) {
	try {
		unserialize($format . ':' . strlen($class) . ':"' . $class . '":0:{}');
		echo "$class was unserialized" . PHP_EOL;
	} catch (Exception $e) {
		if ($e->getMessage() !== "Unserialization of '$class' is not allowed") {
			echo $e->getMessage() . PHP_EOL;
		}
	}
}

echo "Fin.\n";

?>
--EXPECT--
Serialization of 'Imagick' is not allowed
Serialization of 'MyImagick' is not allowed
Serialization of 'ImagickDraw' is not allowed
Serialization of 'ImagickPixel' is not allowed
Fin.
