<?php

if (!extension_loaded("imagick")) die("skip");


if (isset($imageMagickRequiredVersion)) {

	$versionInfo = \Imagick::getVersion();

	if (array_key_exists("versionNumber", $versionInfo) == false) {
		die("skip unable to determine ImageMagick version.");
	}

	$versionInstalled = $versionInfo["versionNumber"];
	$versionRequiredToCheck = $imageMagickRequiredVersion;
	// For some reason ImageMagick reports version 680 as 1680
	if ($versionInstalled > 1000) {
		$versionRequiredToCheck += 1000;
	}

	if ($versionInstalled < $versionRequiredToCheck) {
		die("skip test installed ImageMagick $versionInstalled <= $versionRequiredToCheck");
	}
}


function requireFormat($requiredFormat)
{
	$formats = \Imagick::queryformats();

	foreach ($formats as $format) {
		if (strcasecmp($format, $requiredFormat) === 0) {
			return;
		}
	}

	die("skip test suite requires format $requiredFormat but not available");

}

requireFormat("png");
requireFormat("jpg");


?>
