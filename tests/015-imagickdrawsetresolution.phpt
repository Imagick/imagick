--TEST--
Test ImagickDraw->setResolution
--SKIPIF--
<?php 
	require_once(dirname(__FILE__) . '/skipif.inc');
	checkFormatPresent('png');
?>
--FILE--
<?php

$im = new Imagick();
$im->newImage(1000,1000, "white","png");

$draw = new ImagickDraw();
$draw->setFont (dirname (__FILE__) . '/anonymous_pro_minus.ttf');
$draw->setFontSize(72);

$draw->setResolution(10, 10);
$small = $im->queryFontMetrics($draw, "Hello World");

$draw->setResolution(300, 300);
$large = $im->queryFontMetrics($draw, "Hello World");

if ($small['textWidth'] < $large['textWidth']) {
	echo "Small font _is_ smaller than big font.".PHP_EOL;
}

//These will both be one line.
$oneLine = $im->queryFontMetrics($draw, "Hello Hello");
$forceOneLine = $im->queryFontMetrics($draw, "Hello \nHello", false);

//These will both be multiline
$forceMultiLine = $im->queryFontMetrics($draw, "Hello \nHello", true);
$guessLine = $im->queryFontMetrics($draw, "Hello\nHello");

if (abs($oneLine["textHeight"] - $forceOneLine["textHeight"]) > 0.1) {
	//Reaching this is bad
	echo "One line and forced one line are not the same height.".PHP_EOL;
	echo $oneLine["textHeight"]." ".$forceOneLine["textHeight"].PHP_EOL;
}

if ($forceMultiLine["textHeight"] - (2 * $forceOneLine["textHeight"]) + 2 > 0) {
	echo "Two lines are 2 times one line.".PHP_EOL;
}

if ($guessLine["textHeight"] - (2 * $forceOneLine["textHeight"]) + 2 > 0) {
	echo "Two lines are 2 times one line.".PHP_EOL;
}

echo "OK\n";

?>
--EXPECT--
Small font _is_ smaller than big font.
Two lines are 2 times one line.
Two lines are 2 times one line.
OK
