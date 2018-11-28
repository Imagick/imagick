--TEST--
Test Imagick, setRegistry and getRegistry
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

require_once(dirname(__FILE__) . '/functions.inc');

$tmpPath = Imagick::getRegistry("temporary-path");
if ($tmpPath == null) {
	//I am unsure if this is guaranteed - it might be set via policy.xml
	echo "Temporary-path was empty at start.".PHP_EOL;
}

$currentPath = realpath(dirname(__FILE__));

Imagick::setRegistry("temporary-path", $currentPath);

$tmpPath = Imagick::getRegistry("temporary-path");
if ($tmpPath === false) {
	echo "Failed to set temporary-path".PHP_EOL;
}
else if ($tmpPath == $currentPath) {
	echo "Temporary path was set correctly.".PHP_EOL;
}

$registry = Imagick::listRegistry();

if (array_key_exists("temporary-path", $registry) == true) {

	if ($registry["temporary-path"] === $currentPath) {
		echo "Temporary path was listed correctly.".PHP_EOL;
	}
}

// Since 6.9.9-26, no exception raised
$exceptionExpected = true;

if (isVersionGreaterEqual('6.9.9-26', '7.0.7-15')) {
    $exceptionExpected = false;
}

try {
	$tmpPath = Imagick::getRegistry("non-existent string");

	if ($exceptionExpected === true) {
		echo "Expected exception not thrown.\n";
	}
	else {
		echo "This is fine.";
	}
}
catch (\ImagickException $ie) {
	if ($exceptionExpected === true) {
		echo "This is fine.";
	}
	else {
		echo "Unexpected exception" . $ie->getMessage() . "\n";
	}
}


?>
--EXPECTF--
Temporary-path was empty at start.
Temporary path was set correctly.
Temporary path was listed correctly.
This is fine.
