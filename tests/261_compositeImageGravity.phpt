--TEST--
Test compositeImageGravity
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

$im1 = new Imagick("magick:logo");


$im2 = new Imagick("magick:logo");

$im2->scaleImage(
	$im2->getImageWidth() / 2,
	$im2->getImageHeight() / 2
);

$im1->compositeImageGravity(
	$im2,
	\Imagick::COMPOSITE_ATOP,
	\Imagick::GRAVITY_NORTHEAST
);

$im1->compositeImageGravity(
	$im2,
	\Imagick::COMPOSITE_ATOP,
	\Imagick::GRAVITY_SOUTH
);

// $im1->setImageFormat('png');
// $im1->writeImage('compositeImageGravity.png');

echo "Ok";

?>
--EXPECT--
Ok