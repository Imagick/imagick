--TEST--
Prevent zero dimension images and check exception
--SKIPIF--
<?php 

require_once(dirname(__FILE__) . '/skipif.inc');


?>
--FILE--
<?php

$im = new Imagick();

$im->newPseudoImage(0, 100, "magick:logo");
$im->newPseudoImage(100, 0, "magick:logo");

ini_set("imagick.allow_zero_dimension_images", 1);
$im->newPseudoImage(0, 100, "magick:logo");
echo "Well done, you have a zero dimension image. Now what?\n";

echo "Ok";

?>
--EXPECTF--
Deprecated: Creating images with zero columns is deprecated. If you think you need to do this, please open an issue at https://phpimagick.com/issues in %s on line %d

Deprecated: Creating images with zero rows is deprecated. If you think you need to do this, please open an issue at https://phpimagick.com/issues in %s on line %d
Well done, you have a zero dimension image. Now what?
Ok