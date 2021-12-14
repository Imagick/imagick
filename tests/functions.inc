<?php

/**
 *
 * Gets the installed version of ImageMagick and compares the
 * appropriate version to the installed version. *
 *
 * @param $testIm6Version
 * @param $im7Version
 * @return int
 */
function version_compare_imagemagick($testIm6Version, $im7Version)
{
    $versionInfo = \Imagick::getVersion();

    if (array_key_exists("versionString", $versionInfo) == false) {
        die("skip unable to determine ImageMagick version.");
    }

    $versionInstalledStringComplete = $versionInfo["versionString"];

    $firstSpace = strpos($versionInstalledStringComplete, ' ');
    if ($firstSpace === false) {
        die("Failed to understand version string [$versionInstalledStringComplete] - finding first space");
    }

    $secondSpace = strpos($versionInstalledStringComplete, ' ', $firstSpace + 1);
    if ($secondSpace === false) {
        die("Failed to understand version string [$versionInstalledStringComplete] - finding second space");
    }

    $versionInstalledString = substr($versionInstalledStringComplete, $firstSpace + 1, $secondSpace - $firstSpace - 1);
    // echo "versionInstalledString is $versionInstalledString \n";

    $versionToCompare = $im7Version;
    if (substr($versionInstalledString, 0, 1) === '6') {
        $versionToCompare = $testIm6Version;
    }

    return version_compare($versionInstalledString, $versionToCompare);
}

/**
 *
 * Compares the installed version of ImageMagick and returns true if the appropriate
 * version is greater
 *
 * @param $testIm6Version
 * @param $im7Version
 * @return bool
 */
function isVersionGreaterEqual($testIm6Version, $im7Version)
{
    $versionCompare = version_compare_imagemagick($testIm6Version, $im7Version);
    // echo "version compare for $testIm6Version, $im7Version is $versionCompare \n";

    if ($versionCompare >= 0) {
        return true;
    }

    return false;
}

/**
 * On some systems, where the standard fonts aren't available, trying
 * to draw any text fails as the ImageMagick default font is null.
 *
 * This function just find a 'sensible' font to use, either from the
 * preferred list, or just the first one from queryFonts(). That 'probably'
 * is the right thing to do, as it makes the tests more stable.
 */
function findDefaultFont()
{
    $knownFonts = [
        'Courier',
        'Helvetica',
        'Times-Roman',
        'Liberation-Mono',
        'Utopia',
    ];

    $fontList = \Imagick::queryFonts();

    foreach ($knownFonts as $knownFont) {

        if (in_array($knownFont, $fontList, true) === true) {
            return $knownFont;
        }
    }

    if (count($fontList) !== 0) {
        return $fontList[0];
    }

    throw new \Exception("No fonts available on system, apparently.");
}

// Find and set a font for the Imagick object
function setFontForImagick(\Imagick $imagick)
{
    $font = findDefaultFont();

    $imagick->setFont($font);
}

// Find and set a font for the ImagickDraw object
function setFontForImagickDraw(\ImagickDraw $imagickDraw)
{
    $font = findDefaultFont();

    $imagickDraw->setFont($font);
}


function check_value(array $values, $name, $expected_value)
{
	if (array_key_exists($name, $values) !== true) {

		$message = "Expected key '$name' not set. Array contains:\n";
		$message .= var_export($values, true);

		throw new \Exception($message);
	}


	$value = $values[$name];

	$epsilon = 0.01;

	if (($value < $expected_value - $epsilon) || ($value > $expected_value + $epsilon)) {
		$message = "Value for $name doesn't match expected. Expected: $expected_value, actual: $value";
		throw new \Exception($message);
	}

	echo "Value for '$name' is $value which is close enough to expected $expected_value\n";
}


function check_value_with_epsilon(array $values, $name, $expected_value, $epsilon)
{
	if (array_key_exists($name, $values) !== true) {

		$message = "Expected key '$name' not set. Array contains:\n";
		$message .= var_export($values, true);

		throw new \Exception($message);
	}


	$value = $values[$name];

	if (($value < $expected_value - $epsilon) || ($value > $expected_value + $epsilon)) {
		$message = "Value for $name doesn't match expected. Expected: $expected_value, actual: $value, epsilon = $epsilon";
		throw new \Exception($message);
	}

	echo "Value for '$name' is $value which is close enough to expected $expected_value\n";
}