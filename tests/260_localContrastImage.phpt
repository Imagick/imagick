--TEST--
Test localContrastImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc');

$v = Imagick::getVersion();
if ($v['versionNumber'] < 0x693)
	die ('skip too old ImageMagick');

?>
--FILE--
<?php

$im = new Imagick("magick:logo");
$im->localContrastImage(2, 2);

echo "Ok";

?>
--EXPECT--
Ok