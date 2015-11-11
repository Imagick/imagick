--TEST--
Test getColor and getColorQuantum
--SKIPIF--
<?php 

require_once(dirname(__FILE__) . '/skipif.inc');


?>
--FILE--
<?php

define('ORIGINAL', 'ORIGINAL');
define('NORMALISED', 'NORMALISED');
define('NORMALISED_INCLUDING_ALPHA', 'NORMALISED_INCLUDING_ALPHA');
define('QUANTUM', 'QUANTUM');

function checkExpectedValue($expectedValue, $actualValue, $hasVariance) {

	$variance = 0; 

	// Behaviour of 50% pixel was changed in 
	// key = version
	// value = variance expected in result
	$troubledVersions = array(
		0x692 => 1
	);
	$v = Imagick::getVersion();
	$versionNumber = $v['versionNumber'];

	if (array_key_exists($versionNumber, $troubledVersions)) {
		$variance = $troubledVersions[$versionNumber];
	}

	if (Imagick::getHDRIEnabled()) {
		return abs($expectedValue - $actualValue) < (0.01 + $variance);
	}

	if ($hasVariance) {
		$difference = abs($expectedValue - $actualValue);
		if ($difference < 1 + $variance) {
			return true;
		}
		echo "difference $difference not < 1 + variance $variance\n";
		return false;
	}
	else if($expectedValue == $actualValue) {
		return true;
	}

	return false;
}

function getExpectedValue($someValue) {
	if (Imagick::getHDRIEnabled()) {
		return $someValue;
	}

	$v = Imagick::getVersion();
	if ($v['versionNumber'] >= 0x692) {
		//this is the new correct behaviour
		return (intval(round($someValue, 0, PHP_ROUND_HALF_UP)));
	}
	else {
		//old behaviour had wrong rounding.
		return (intval(round($someValue, 0, PHP_ROUND_HALF_DOWN)));
	}
}


$tests = array(
	array(
		'red',
		ORIGINAL,  
		array(
			array('r', getExpectedValue(255), 0),
			array('a', getExpectedValue(1.0), 0)
		),
	),
	array(
		'red',
		QUANTUM,  
		array(
			array('r', getExpectedValue(\Imagick::getQuantum()), 0),
			array('a', getExpectedValue(\Imagick::getQuantum()), 0)
		),
	),
	array(
		'rgb(25%, 25%, 25%)',
		QUANTUM,
		array(
			array('r', getExpectedValue(\Imagick::getQuantum() / 4), 0),
			array('a', getExpectedValue(\Imagick::getQuantum()), 0),
		)
	)
);

$version = Imagick::getVersion();
// The following don't seem stable in lesser versions.
if ($version['versionNumber'] >= 0x687) {
	$tests[] = array(
		'green',
		QUANTUM,  
		array(
			array('g', getExpectedValue(\Imagick::getQuantum() * (128 / 255)), 1),
			array('a', getExpectedValue(\Imagick::getQuantum()), 1)
		),
	);

	$tests[] = array(
		'rgb(0, 50%, 0)',
		QUANTUM,  
		array(
			array('g', getExpectedValue(\Imagick::getQuantum() / 2), 1),
			array('a', getExpectedValue(\Imagick::getQuantum()), 0)
		),
	);
}


foreach ($tests as $test) {

	list($colorString, $type, $expectations) = $test;
	$pixel = new ImagickPixel($colorString);

	switch ($type) {

		case(ORIGINAL): {
			$color = $pixel->getColor();
			break;
		}

		case(NORMALISED): {
			$color = $pixel->getColor(true);
			break;
		}

		case(NORMALISED_INCLUDING_ALPHA): {
			$color = $pixel->getColor(2);
			break;
		}

		case(QUANTUM): {
			$color = $pixel->getColorQuantum();
			break;
		}

		default:{
			echo "Unknown test type $type" . PHP_EOL;
			break;
		}
	}

	foreach ($expectations as $test) {
		list($key, $expectedValue, $hasVariance) = $test;
		if (!checkExpectedValue($expectedValue, $color[$key], $hasVariance)) {
			printf( 
				"%s %s is wrong for colorString '%s': actual %s != expected %s"  . PHP_EOL,
				$type,
				$key, $colorString,
				$color[$key], $expectedValue
			);
		}
	}
}

echo "OK" . PHP_EOL;
?>
--EXPECT--
OK