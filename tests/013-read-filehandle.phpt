--TEST--
Imagick::readImageFile test
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$file = dirname(__FILE__) . '/__tmp_rose.gif';

$imagick = new Imagick('magick:rose');
$imagick->setImageFormat('gif');
$imagick->writeImage($file);

$imagick->clear();

$handle = fopen($file, 'rb');
$imagick->readImageFile($handle);

unlink($file);

echo 'success';

?>
--EXPECT--
success