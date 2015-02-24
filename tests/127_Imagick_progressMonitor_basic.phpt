--TEST--
Test Imagick, progressMonitor
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$radius = 5;
$sigma = 1;

$status = 'Not cancelled';
$startTime = time();

$callback = function ($offset, $span) use (&$status, $startTime) {
	if (((100 * $offset) / $span)  > 20) {
		$status = "Processing cancelled";
		return false;
	}

	$nowTime = time();

	if ($nowTime - $startTime > 5) {
		$status = "Processing cancelled";
		return false;
	}

	return true;
};

$imagick = new \Imagick();
$imagick->newPseudoImage(640, 480, "magick:logo");

$imagick->setProgressMonitor($callback);

try {

	$imagick->charcoalImage($radius, $sigma);
	$bytes = $imagick->getImageBlob();
	echo "Progress monitor failed to interrupt.";
}
catch(\Exception $e) {
	echo $status.PHP_EOL;
}
?>
--EXPECTF--
Processing cancelled