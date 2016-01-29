--TEST--
Test Imagick, Imagick::evaluateImages
--SKIPIF--
<?php

require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

function im_check($value) {
	static $count = 0;

	if (abs($value - 0.5) > 0.0000001) {
		echo "Unexpected value of $value for check $count\n";
	}

	$count++;
}


$imagick = new \Imagick();
$imagick->newPseudoImage(640, 480, "magick:logo");

$imagick->setImageFormat('png');
$v = Imagick::getVersion();
if ($v['versionNumber'] < 0x700) {
	$imagick->setImageRedPrimary(0.5, 0.5);
	$imagick->setImageGreenPrimary(0.5, 0.5);
	$imagick->setImageBluePrimary(0.5, 0.5);
	$imagick->setImageWhitePoint(0.5, 0.5);

	$color = $imagick->getImageRedPrimary();
	im_check($color['x']);im_check($color['y']);

	$color = $imagick->getImageGreenPrimary();
	im_check($color['x']);im_check($color['y']);

	$color = $imagick->getImageBluePrimary();
	im_check($color['x']);im_check($color['y']);

	$color = $imagick->getImageWhitePoint();
	im_check($color['x']);im_check($color['y']);
}
else {
	$imagick->setImageRedPrimary(0.5, 0.5, 0.5);
	$imagick->setImageGreenPrimary(0.5, 0.5, 0.5);
	$imagick->setImageBluePrimary(0.5, 0.5, 0.5);
	$imagick->setImageWhitePoint(0.5, 0.5, 0.5);

	$color = $imagick->getImageRedPrimary();
	im_check($color['x']);im_check($color['y']);im_check($color['z']);

	$color = $imagick->getImageGreenPrimary();
	im_check($color['x']);im_check($color['y']);im_check($color['z']);

	$color = $imagick->getImageBluePrimary();
	im_check($color['x']);im_check($color['y']);im_check($color['z']);

	$color = $imagick->getImageWhitePoint();
	im_check($color['x']);im_check($color['y']);im_check($color['z']);
}


echo "Ok";
?>
--EXPECTF--
Ok