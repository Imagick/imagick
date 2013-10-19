--TEST--
Test ImagickPixel::isSimilar
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc');

--FILE--
<?php

$root3 = 1.732050807568877;

$tests = array(
	['rgb(245, 0, 0)',     'rgb(255, 0, 0)',        9 / $root3, false,],
	['rgb(245, 0, 0)',     'rgb(255, 0, 0)',       10 / $root3, true,],
	['rgb(0, 0, 0)',       'rgb(7, 7, 0)',          9 / $root3, false,],
	['rgb(0, 0, 0)',       'rgb(7, 7, 0)',         10 / $root3, true,],
	['rgba(0, 0, 0, 1)',   'rgba(7, 7, 0, 1)',      9 / $root3, false,],
	['rgba(0, 0, 0, 1)',   'rgba(7, 7, 0, 1)',     10 / $root3, true,],
	['rgb(128, 128, 128)', 'rgb(128, 128, 120)',    7 / $root3, false,],
	['rgb(128, 128, 128)', 'rgb(128, 128, 120)',    8 / $root3, true,],
	
	['rgb(0, 0, 0)',       'rgb(255, 255, 255)',  254.9,        false,],
	['rgb(0, 0, 0)',       'rgb(255, 255, 255)',    255,        true,],
	['rgb(255, 0, 0)',     'rgb(0, 255, 255)',    254.9,        false,],
	['rgb(255, 0, 0)',     'rgb(0, 255, 255)',      255,        true,],
	['black',              'rgba(0, 0, 0)',         0.0,        true],
	['black',              'rgba(10, 0, 0, 1.0)',  10.0 / $root3, true],
);

try {
	foreach ($tests as $testInfo) {
		$color1 = $testInfo[0];
		$color2 = $testInfo[1];
		$distance = $testInfo[2];
		$expectation = $testInfo[3];
		$testDistance = ($distance / 255.0);

		$color1Pixel = new ImagickPixel($color1);
		$color2Pixel = new ImagickPixel($color2);

		$isSimilar = $color1Pixel->isSimilar($color2Pixel, $testDistance);

		if ($isSimilar !== $expectation) {
			echo "isSimilar failed. Color [$color1] compared to color [$color2] is not within distance $testDistance.". PHP_EOL;
		}
	}
	echo "success";
} catch (\Exception $e) {
	echo "Exception caught in ImagickPixel::isSimilar test: ".$e->getMessage() . PHP_EOL;
}

?>
--EXPECT--
success