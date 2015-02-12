--TEST--
ImagickKernel::fromArray test
--SKIPIF--
<?php 

$imageMagickRequiredVersion = 680;
require_once(dirname(__FILE__) . '/skipif.inc');
 
?>
--FILE--
<?php


$kernel = [
	[1, 0, 0],
	[0, 0, -2],
];

$kernel = ImagickKernel::fromArray($kernel);

$kernel->scale(1, \Imagick::NORMALIZE_KERNEL_VALUE);

$kernel->addUnityKernel(0.50);

$imagick = new Imagick(realpath("Biter_500.jpg"));

$imagick->morphology(
	\Imagick::MORPHOLOGY_CONVOLVE,
	1,
	$kernel
);

echo "Ok";

?>
--EXPECTF--
Ok