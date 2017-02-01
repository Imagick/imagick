<?php

$headerFilename = __DIR__."/../php_imagick.h";

$phpImagickLines = file($headerFilename);

if ($phpImagickLines === false) {
    echo "Failed to read $headerFilename\n";
    exit(-1);
}

$versionPackage = false;
$extNumPackage = false;

foreach ($phpImagickLines as $phpImagickLine) {
    if (strpos($phpImagickLine, "#define PHP_IMAGICK_VERSION") === 0) {
        $versionPackage = trim(substr($phpImagickLine, strlen("#define PHP_IMAGICK_VERSION")));
    }

    if (strpos($phpImagickLine, "#define PHP_IMAGICK_EXTNUM") === 0) {
        $extNumPackage = trim(substr($phpImagickLine, strlen("#define PHP_IMAGICK_EXTNUM")));
    }
}


$sxe = simplexml_load_file (__DIR__ . "/../package.xml");

$versionRelease =  (string) $sxe->version->release;
$versionApi = (string) $sxe->version->api;
$stabilityRelease =  (string) $sxe->stability->release;
$stabilityApi = (string) $sxe->stability->api;

$versionReleaseNumber = preg_replace('/[^\d\.]/', '', $versionApi);

$parts = explode('.', $versionReleaseNumber);

if (count($parts) != 3) {
    echo "Version number doesn't have 3 parts.";
    exit(-1);
}

list($major, $minor, $bug) = $parts;

$extNum = sprintf("%d%02d%02d", $major, $minor, $bug);

if (strcasecmp($extNumPackage, $extNum) !== 0) {
    echo "Error: package.xml and php_imagick.h EXTNUM mismatch: $extNumPackage, $extNum\n";
	exit (-1);
}

if (strcasecmp($stabilityRelease, "beta") === 0) {
    if (strpos($versionRelease, 'b') === false && strpos($versionRelease, 'rc') === false) {
        echo "Error: release is marked 'beta' but version [$versionRelease] doesn't include b or RC\n";
        exit(-1);
    }
}
else if (strcasecmp($stabilityRelease, "stable") === 0) {
    if (strpos($versionRelease, 'b') !== false) {
        echo "Error: release is marked 'stable' but version [$versionRelease] includes b\n";
        exit(-1);
    }
    if (strpos($versionRelease, 'rc') !== false) {
        echo "Error: release is marked 'stable' but version [$versionRelease] includes RC \n";
        exit(-1);
    }
}
else {
    echo "unknown stability $stabilityRelease\n";
    exit(-1);
}

if (strcmp($stabilityRelease, $stabilityApi) !== 0) {
    echo "Warning: API and release stability differ";
    exit(-1);
}
