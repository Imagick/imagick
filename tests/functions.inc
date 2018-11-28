<?php

/**
 *
 * Compares the installed version of ImageMagick and compares the
 * appropriate version
 *
 *
 * @param $testIm6Version
 * @param $im7Version
 * @return bool
 */
function isVersionGreaterEqual($testIm6Version, $im7Version)
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

    $versionToCompare = $im7Version;
    if (substr($versionInstalledString, 0, 1) === '6') {
        $versionToCompare = $testIm6Version;
    }

    if (version_compare($versionInstalledString, $versionToCompare) >= 0) {
        return true;
    }

    return false;
}
