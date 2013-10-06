--TEST--
Test convolveImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php
$im = new Imagick ('magick:rose');

for ($i = 1; $i < 40; $i++) {
	$n = $i * $i;

	try {
		$im->convolveImage (array_fill (0, $n, 1));
	} catch (Exception $e) {
		echo "Failed for $n" . PHP_EOL;
	}
}
echo "OK" . PHP_EOL;

?>
--EXPECT--
OK