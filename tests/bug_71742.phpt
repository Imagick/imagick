--TEST--
Bug #71742	polyline touched by array_walk
--SKIPIF--
<?php 
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$coordinates = array();

foreach (range (0, 100) as $index) {
	$coordinates[] = array(
		'x' => 2 * $index,
		'y' => pow($index, 2)
	);
}

$callback = function (&$coordinate) {
	$coordinate['y'] = 200 - $coordinate['y'] / 50;
};

array_walk($coordinates, $callback);

$imagick = new Imagick();
$imagick->newImage(200, 200, "white");

$draw = new ImagickDraw ();
$draw->setFillColor("none");
$draw->setStrokeColor("black");

//Fatal error in PHP 7, but not in PHP <= 5.6
$draw->polyline($coordinates);

$draw->translate(0, -20);
////Works in PHP 7
$draw->polyline (array_values($coordinates));
$imagick->drawImage($draw);
//$imagick->writeImage(getcwd()."/test.png");
$imagick->setImageFormat('png');
$bytes = $imagick->getImageBlob();

if (strlen($bytes) <= 0) { 
	echo "Failed to generate image.";
}

//$imagick->writeImage("./bugTest.png");

echo "Ok";

?>
--EXPECT--
Ok