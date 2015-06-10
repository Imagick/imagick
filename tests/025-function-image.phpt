--TEST--
Test functionimage
--SKIPIF--
<?php
$imageMagickBelowVersion = 0x700;
require_once(dirname(__FILE__) . '/skipif.inc');

?>
--FILE--
<?php
$im = new Imagick ('magick:rose');
$im->convolveimage (array (1, 'a', 1));

echo "OK" . PHP_EOL;
?>
--EXPECT--
OK