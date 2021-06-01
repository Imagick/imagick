--TEST--
Prevent zero dimension images and check exception
--SKIPIF--
<?php 

require_once(dirname(__FILE__) . '/skipif.inc');


?>
--FILE--
<?php

$im = new Imagick();

try {
	$im->newPseudoImage(0, 100, "magick:logo");
	echo "zero columns failed to throw.";
}
catch(\ImagickException $ie) {
	if (strpos($ie->getMessage(), "Cannot create image with zero columns.") !== 0) {
		echo "Exception message appears wrong: " . $ie->getMessage() . "\n";
	}
}


try {
	$im->newPseudoImage(100, 0, "magick:logo");
	echo "zero row failed to throw.";
}
catch(\ImagickException $ie) {
	if (strpos($ie->getMessage(), "Cannot create image with zero rows.") !== 0) {
		echo "Exception message appears wrong: " . $ie->getMessage() . "\n";
	}
}

ini_set("imagick.allow_zero_dimension_images", 1);
$im->newPseudoImage(0, 100, "magick:logo");
echo "Well done, you have a zero dimension image. Now what?\n";

echo "Ok";

?>
--EXPECTF--
Well done, you have a zero dimension image. Now what?
Ok