--TEST--
Test filling thumbnail with color
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>

$v = Imagick::getVersion();
if ($v['versionNumber'] < 0x632) {
	die ('skip too old ImageMagick');
}
--FILE--
<?php

$im = new Imagick("magick:logo");
$im->setImageBackgroundColor("pink");
$im->thumbnailImage(200, 200, true, true);

$color = $im->getImagePixelColor(5, 5);
var_dump($color->isSimilar("pink", 0));

$color = $im->getImagePixelColor(199, 5);
var_dump($color->isSimilar("pink", 0));

?>
--EXPECTF--
bool(true)
bool(true)