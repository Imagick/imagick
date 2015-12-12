--TEST--
Test for round issues
--SKIPIF--
<?php 
require_once(dirname(__FILE__) . '/skipif.inc'); 
if (getenv('SKIP_SLOW_TESTS')) die('skip slow tests excluded by request');
?>
--FILE--
<?php

//Test the the calculated values are actually correct.
$desired_height = 250;
$imageWidth = 1128;

//Test the the calculated values are actually correct.
$desired_height = 250;
$imageWidth = 1128;
$imageHeight = 1128;

$legacySettings = array(0, 1);

foreach($legacySettings as $legacy) {
	for ($desired_width = 245; $desired_width < 255; $desired_width++) {
		$imagick = new Imagick();
		$imagick->newPseudoImage($imageWidth, $imageHeight, 'xc:white');

		$imagick->cropThumbnailImage(
			$desired_width, $desired_height,
			$legacy
		);
		$error = false;

		$thumbnailImageWidth = $imagick->getImageWidth();
		$thumbnailImageHeight = $imagick->getImageHeight();

		if ($thumbnailImageHeight != $desired_height) {
			echo "Incorrect height for desired_width $desired_width imageHeight $imageHeight".PHP_EOL;
			$error = true;
		}

		$expectedWidth = $desired_width;
		$expectedHeight = $desired_height;

		if ($legacy == true && 
			$desired_width == 250 &&
			$desired_height == 250) {
			// Thumbnail size of 249 x 250 does not matched desired size 250 x 250 for source image of 1128 x 1128
			$expectedWidth = 249;
		}

		if ($thumbnailImageWidth != $expectedWidth) {
			echo "Incorrect width for desired_width $desired_width imageHeight $imageHeight".PHP_EOL;
			$error = true;
		}

		if ($thumbnailImageHeight != $expectedHeight) {
			echo "Incorrect width for desired_width $desired_width imageHeight $imageHeight".PHP_EOL;
			$error = true;
		}

		if ($error) {
			printf(
				"Thumbnail size of %d x %d does not matched expected size %d x %d for source image of %d x %d. Legacy is %d\n",
				$thumbnailImageWidth, $thumbnailImageHeight,
				$desired_width, $desired_height,
				$imageWidth, $imageHeight,
				$legacy
			);
		}
	}
}


echo "Done" . PHP_EOL;

?>
--EXPECTF--
Done