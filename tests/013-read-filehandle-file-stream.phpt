--TEST--
Imagick::readImageFile, file stream test
--SKIPIF--
<?php
	require_once(dirname(__FILE__) . '/skipif.inc');
	checkFormatPresent('jpg');
?>
--FILE--
<?php

$file = dirname(__FILE__) . '/__tmp_rose.jpg';
$handle = fopen($file, 'w+');

$imagick = new Imagick('magick:rose');
$imagick->setImageFormat('jpg');
$imagick->writeImageFile($handle);

$imagick->clear();

rewind($handle);

$imagick->readImageFile($handle);

unlink($file);

echo 'success';

?>
--CLEAN--
<?php
@unlink(dirname(__FILE__) . '/__tmp_rose.jpg');
?>
--EXPECT--
success
