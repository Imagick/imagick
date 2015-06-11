--TEST--
Test clone length, this is expected upstream behaviour
--SKIPIF--
<?php 
require_once(dirname(__FILE__) . '/skipif.inc');

checkFormatPresent('png');

$v = imagick::getversion ();

if ($v ['versionNumber'] >= 0x640 && $v ['versionNumber'] < 0x650)
	die ('skip seems to be different in this version of ImageMagick');
?>
--FILE--
<?php

$im = new Imagick ('magick:rose');
$im->setImageFormat ('png');
if ($im->getImageLength()) {
	echo "Image created has length" . PHP_EOL;
}
else {
	echo "Image created has zero length" . PHP_EOL;
}

$cloned = clone $im;
$cloned->setImageFormat ('png');

var_dump ($cloned->getImageLength ());

?>
--EXPECT--
Image created has length
int(0)
