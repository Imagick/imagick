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



