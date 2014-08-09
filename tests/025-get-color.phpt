--TEST--
Test functionimage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

define('ORIGINAL', 'ORIGINAL');
define('NORMALISED', 'NORMALISED');
define('NORMALISED_INCLUDING_ALPHA', 'NORMALISED_INCLUDING_ALPHA');
define('QUANTUM', 'QUANTUM');



$tests = array(
	array(
		'red',
		ORIGINAL,  
		array(
			'r' => 255,
			'a' => 1.0
		),
	),
	array(
		'red',
		QUANTUM,  
		array(
			'r' => \Imagick::getQuantum(),
			'a' => \Imagick::getQuantum()
		),
	),
	array(
		'rgb(25%, 25%, 25%)',
		QUANTUM,
		array(
			'r' => (intval(round(\Imagick::getQuantum() / 4))),
			'a' => \Imagick::getQuantum()
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

	foreach ($expectations as $key => $value) {
		if ($color[$key] != $value) {
			printf( 
				"%s %s is wrong for colorString '%s': %s != %s"  . PHP_EOL,
				$type,
				$key, $colorString,
				$color[$key], $value
			);
		}
	}
}



echo "OK" . PHP_EOL;
?>
--EXPECT--
OK