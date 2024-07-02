--TEST--
Imagick::readImageFile, in-memory stream test
--SKIPIF--
<?php
	require_once(dirname(__FILE__) . '/skipif.inc');
	checkFormatPresent('jpg');
?>
--FILE--
<?php

$handle = fopen('php://memory', 'w+');

$imagick = new Imagick('magick:rose');
$imagick->setImageFormat('jpg');
$imagick->writeImageFile($handle);

$imagick->clear();

rewind($handle);

$imagick->readImageFile($handle);

echo "Width: " . $imagick->getImageWidth() . "\n";
echo "Height: " . $imagick->getImageHeight() . "\n";

echo 'success';

?>
--EXPECT--
Width: 70
Height: 46
success
