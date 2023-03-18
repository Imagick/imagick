--TEST--
Test Imagick, clutImageWithInterpolate
--SKIPIF--
<?php

require_once(dirname(__FILE__) . '/skipif.inc');

checkClassMethods('Imagick', array('clutImageWithInterpolate'));

?>
--FILE--
<?php

$draw = new \ImagickDraw();
$draw->setStrokeOpacity(0);
$draw->setFillColor('black');
$points = [
	['x' => 40 * 3, 'y' => 10 * 5],
	['x' => 20 * 3, 'y' => 20 * 5],
	['x' => 70 * 3, 'y' => 50 * 5],
	['x' => 80 * 3, 'y' => 15 * 5],
];
$draw->polygon($points);
$imagick = new \Imagick();

$imagick->setColorspace(\Imagick::COLORSPACE_GRAY);

$imagick->newPseudoImage(
	300, 300,
	"xc:white"
);


$imagick->drawImage($draw);
$imagick->blurImage(0, 10);

$draw = new \ImagickDraw();
$draw->setStrokeOpacity(1);
$draw->setFillColor('red');
$draw->point(0, 2);
$draw->setFillColor('yellow');
$draw->rectangle(0, 0, 1, 1);
$gradient = new Imagick();
$gradient->newPseudoImage(1, 5, 'xc:black');
$gradient->drawImage($draw);


$imagick->setImageFormat('png');
$imagick->clutImageWithInterpolate($gradient, Imagick::INTERPOLATE_BILINEAR);

$bytes = $imagick->getImageBlob();
if (strlen($bytes) <= 0) { echo "Failed to generate image.";}

// $imagick->writeImage(__DIR__ . "/feels_bad_man.png");

echo "Ok";
?>
--EXPECTF--
Ok