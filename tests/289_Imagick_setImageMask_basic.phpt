--TEST--
Test Imagick, medianFilterImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('getImageMask', 'setImageMask'));
?>
--FILE--
<?php

$canvas = new Imagick(__DIR__ . '/houghline_input_image.png');

$mask = new Imagick();
$mask->newPseudoImage(
	$canvas->getImageWidth(),
	$canvas->getImageHeight(),
	'xc:black'
);

$drawing = new ImagickDraw();
$drawing->setBorderColor('black');
$drawing->setFillColor('black');
$drawing->rectangle(0, 0, $mask->getImageWidth(), $mask->getImageHeight());

$drawing->setBorderColor('white');
$drawing->setFillColor('white');
$drawing->circle(
	$mask->getImageWidth() / 2,
	$mask->getImageHeight() / 2,
	2 * $mask->getImageWidth() / 3,
	$mask->getImageHeight() / 2
);

$mask->drawImage($drawing);
//$mask->writeImage(__DIR__ . "/test_mask.png");

// This would probably be more useful for users
// but shows the issue with PIXELMASK_COMPOSITE
// $mask->blurImage(10, 2);

$mask_types = array(
	\Imagick::PIXELMASK_READ =>        "PIXELMASK_READ",
	\Imagick::PIXELMASK_WRITE =>       "PIXELMASK_WRITE",
	\Imagick::PIXELMASK_COMPOSITE =>   "PIXELMASK_COMPOSITE",
);

$channel_types = array(
	\Imagick::CHANNEL_ALL => "CHANNEL_ALL",
	\Imagick::CHANNEL_RED => "CHANNEL_RED",
	\Imagick::CHANNEL_ALPHA => "CHANNEL_ALPHA",
	\Imagick::CHANNEL_RGBA => "CHANNEL_RGBA",
	\Imagick::CHANNEL_BLACK => "CHANNEL_BLACK",
	\Imagick::CHANNEL_DEFAULT => "CHANNEL_DEFAULT",
);

foreach ($channel_types as $channel_type => $channel_name) {
	foreach ($mask_types as $type => $type_name) {
		$output = clone $canvas;
		$output->setImageMask($mask, $type);

		$output->blurImage(15, 4, $channel_type);
//		$output->writeImage(__DIR__ . "/test_canvas" . $type_name . "_" . $channel_name .  ".png");
	}
}

echo "Ok";
?>
--EXPECTF--
Ok