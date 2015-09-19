--TEST--
Test Imagick, progressMonitor
--SKIPIF--
<?php

require_once(dirname(__FILE__) . '/skipif.inc'); 
checkClassMethods('Imagick', array('setProgressMonitor'));
?>
--FILE--
<?php

$radius = 5;
$sigma = 1;

if (property_exists('Imagick', 'RESOURCETYPE_THREAD')) {
	Imagick::setResourceLimit(\Imagick::RESOURCETYPE_THREAD, 8);
}

$debug = "";
$status = 'Not cancelled';
$startTime = time();

$callback = function ($offset, $span) use (&$status, $startTime, $debug) {

	static $x = 0;

	if (((100 * $offset) / $span)  > 20) {
		$status = "Processing cancelled";
		return false;
	}

	$nowTime = time();

	$debug .= "$x: nowTime $nowTime - startTime $startTime".PHP_EOL;
	$x++;

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
	echo "Progress monitor failed to interrupt.".PHP_EOL;
	echo $debug;
}
catch(\Exception $e) {
	echo $status.PHP_EOL;
}
?>
--EXPECTF--
Processing cancelled