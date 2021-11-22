--TEST--
Imagick::readImageFile test
--SKIPIF--
<?php 
	require_once(dirname(__FILE__) . '/skipif.inc');
	checkFormatPresent('jpg');
?>
--FILE--
<?php

$file = dirname(__FILE__) . '/__tmp_rose.jpg';

$imagick = new Imagick('magick:rose');
$imagick->setImageFormat('jpg');
$imagick->writeImage($file);

$imagick->clear();

$handle = fopen($file, 'rb');
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