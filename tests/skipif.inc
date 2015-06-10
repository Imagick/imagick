<?php

if (!extension_loaded("imagick")) die("skip");


if (isset($imageMagickRequiredVersion)) {
	$versionInfo = \Imagick::getVersion();

	if (array_key_exists("versionNumber", $versionInfo) == false) {
		die("skip unable to determine ImageMagick version.");
	}

	$versionInstalled = $versionInfo["versionNumber"];

	if ($versionInstalled < $imageMagickRequiredVersion) {
		die("skip ImageMagick installed $versionInstalled <= required $imageMagickRequiredVersion");
	}
}

if (isset($imageMagickBelowVersion)) {
	$versionInfo = \Imagick::getVersion();

	if (array_key_exists("versionNumber", $versionInfo) == false) {
		die("skip unable to determine ImageMagick version.");
	}

	$versionInstalled = $versionInfo["versionNumber"];

	if ($versionInstalled >= $imageMagickBelowVersion) {
		die("skip ImageMagick installed $versionInstalled >= $imageMagickBelowVersion");
	}
}

function checkClassMethods($class, $methods)
{
	foreach ($methods as $method) {
		if (method_exists('Imagick', $method) == false) {
			die("skip Class method $class::$method not present");
		}
	}
}


?>
