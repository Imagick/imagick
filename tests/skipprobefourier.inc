<?php

$canCallFourier = false;

ob_start();

echo("skip failed to probe fourier functions");

try {
	$imagick = new \Imagick();
	$imagick->newPseudoImage(640, 480, "magick:logo");
	$imagick->resizeimage(512, 512, \Imagick::FILTER_LANCZOS, 1);
	$imagick->forwardFourierTransformImage(true);
	$canCallFourier = true;
}
catch(\Exception $e) {
	//fftw probably not available.
}

ob_end_clean();

if ($canCallFourier == false) {
	die("skip fourier function seems unavailable");
}

?>