--TEST--
Test autoOrient
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc');

$v = Imagick::getVersion();
if ($v['versionNumber'] < 0x693)
	die ('skip too old ImageMagick');

// if ($v ['versionNumber'] >= 0x660 && $v ['versionNumber'] < 0x670)
//	die ('skip seems to be broken in this version of ImageMagick');
?>
--FILE--
<?php

$im = new Imagick("magick:logo");
$im->autoOrient();

echo "Ok";

?>
--EXPECT--
Ok