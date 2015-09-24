--TEST--
Test for round issues
--SKIPIF--
<?php 
require_once(dirname(__FILE__) . '/skipif.inc'); 
if (getenv('SKIP_SLOW_TESTS')) die('skip slow tests excluded by request');
?>
--FILE--
<?php

$desired_height = 250;
$imageWidth = 1128;

for ($desired_width=245; $desired_width<255 ; $desired_width++) { 
	for ($imageHeight=1125 ; $imageHeight<1135 ; $imageHeight++) {

		$imagick = new Imagick();

		$imagick->newImage($imageWidth, $imageHeight, '#dddddd', 'MIFF' );
		$imagick->cropThumbnailImage($desired_width, $desired_height);

		if ($imagick->getImageWidth() != $desired_width) {
			printf(
				"Wrong width %d for test case: ".PHP_EOL,
				$imagick->getImageWidth()
			);
			var_dump($testCase);
		}

		if ($imagick->getImageHeight() != $desired_height) {
			printf(
				"Wrong height %d for test case:".PHP_EOL,
				$imagick->getImageHeight()
			);
			var_dump($testCase);
		}

		$imagick->destroy();
		$imagick = null;
	}
}


echo "Done" . PHP_EOL;

?>
--EXPECTF--
Done