--TEST--
Test getColor and getColorQuantum
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

define('ORIGINAL', 'ORIGINAL');
define('NORMALISED', 'NORMALISED');
define('NORMALISED_INCLUDING_ALPHA', 'NORMALISED_INCLUDING_ALPHA');
define('QUANTUM', 'QUANTUM');

function checkExpectedValue($expectedValue, $actualValue) {
	$features = Imagick::getFeatures();
	$features = strtolower($features);

	if (strpos($features, 'hdri') !== false) {
		return fabs($expectedValue - $actualValue) < 0.01;
	}

	if ($expectedValue == $actualValue) {
		return true;
	}

	return false;
}

function getExpectedValue($someValue) {
	$features = Imagick::getFeatures();
	$features = strtolower($features);

	if (strpos($features, 'hdri') !== false) {
		return $someValue;
	}

	return (intval(round($someValue, 0, PHP_ROUND_HALF_DOWN)));
}


$tests = array(
	array(
		'red',
		ORIGINAL,  
		array(
			'r' => getExpectedValue(255),
			'a' => getExpectedValue(1.0)
		),
	),
	array(
		'red',
		QUANTUM,  
		array(
			'r' => getExpectedValue(\Imagick::getQuantum()),
			'a' => getExpectedValue(\Imagick::getQuantum())
		),
	),
	array(
		'green',
		QUANTUM,  
		array(
			'g' => getExpectedValue(\Imagick::getQuantum() * (128 / 255)),
			'a' => getExpectedValue(\Imagick::getQuantum())
		),
	),
	array(
		'rgb(0, 50%, 0)',
		QUANTUM,  
		array(
			'g' => getExpectedValue(\Imagick::getQuantum() / 2),
			'a' => getExpectedValue(\Imagick::getQuantum())
		),
	),
	array(
		'rgb(25%, 25%, 25%)',
		QUANTUM,
		array(
//			'r' => (intval(round(\Imagick::getQuantum() / 4))),
			'r' => getExpectedValue(\Imagick::getQuantum() / 4),
			'a' => getExpectedValue(\Imagick::getQuantum())
		)
	)
);

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

	foreach ($expectations as $key => $expectedValue) {
		if (!checkExpectedValue($expectedValue, $color[$key])) {
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