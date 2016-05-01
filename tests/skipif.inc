<?php

if (!extension_loaded("imagick")) die("skip");


if (isset($imageMagickRequiredVersion)) {
	$versionInfo = \Imagick::getVersion();

	if (array_key_exists("versionNumber", $versionInfo) == false) {
		die("skip unable to determine ImageMagick version.");
	}

	$versionInstalled = $versionInfo["versionNumber"];

	if ($versionInstalled < $imageMagickRequiredVersion) {
		$message = sprintf(
			"skip ImageMagick installed %x <= required %x",
			$versionInstalled,
			$imageMagickRequiredVersion
		);
		die($message);
	}
}


if (isset($imageMagickBelowVersion)) {
	$versionInfo = \Imagick::getVersion();

	if (array_key_exists("versionNumber", $versionInfo) == false) {
		die("skip unable to determine ImageMagick version.");
	}

	$versionInstalled = $versionInfo["versionNumber"];

	if ($versionInstalled >= $imageMagickBelowVersion) {
		$message = sprintf(
			"skip ImageMagick installed %x >= limit check %x",
			$versionInstalled,
			$imageMagickBelowVersion
		);
		die($message);
	}
}


if (isset($imageMagickBelowVersion)) {
	$versionInfo = \Imagick::getVersion();

	if (array_key_exists("versionNumber", $versionInfo) == false) {
		die("skip unable to determine ImageMagick version.");
	}

	$versionInstalled = $versionInfo["versionNumber"];

	if ($versionInstalled >= $imageMagickBelowVersion) {
		$message = sprintf(
			"skip ImageMagick installed %x >= %x",
			$versionInstalled,
			$imageMagickBelowVersion
		);
		die($message);
	}
}

function checkClassMethods($class, $methods)
{
	foreach ($methods as $method) {
		if (method_exists($class, $method) == false) {
			die("skip Class method $class::$method not present");
		}
	}
}

function checkFormatPresent($format)
{
	$result = Imagick::queryFormats(strtoupper($format));
	if (count($result) == false) {
		die("skip format $format not supported by ImageMagick as compiled.");
	}
}

function requirePHP($required)
{
	if (version_compare(PHP_VERSION, $required) < 0) {
		die("skip PHP version $required required, but have ".PHP_VERSION.".");
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
