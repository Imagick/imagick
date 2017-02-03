--TEST--
Testing ImagickPixel with unitialized pixel_wand
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
requirePHP("5.6");
?>
--FILE--
<?php

interface MockInterface {}

class ImagickPixelMock extends \ImagickPixel implements MockInterface
{
    protected $foo1;
    protected $foo2;
    protected $foo3;
    protected $foo4;
}

$reflectionClass = new ReflectionClass('ImagickPixelMock');
$instance = $reflectionClass->newInstanceWithoutConstructor();
$methods = $reflectionClass->getMethods();

$methodsAndParams = array(
    'clear' => [],
    'destroy' => [],
    'getColor' => [],
    'getColorAsString' => [],
    'getColorCount' => [],
    'getColorQuantum' => [],
    'getColorValue' => [Imagick::COLOR_BLUE],
    'getColorValueQuantum' => [Imagick::COLOR_RED],
    'getHSL' => [],
    'getIndex' => [],
    'isPixelSimilar' =>  ['red', 0.1],
    'isPixelSimilarQuantum' =>  ['red', 100],
    'isSimilar' =>  ['red', 0.1],
    'setColor' => ['red'],
    'setcolorcount' => [4],
    'setColorValue' => [Imagick::COLOR_BLUE, 0.5],
    'setColorValueQuantum' => [Imagick::COLOR_BLUE, 1],
    'setHSL' => [0.5, 0.5, 0.5],
    'setIndex' => [5],
    'setcolorfrompixel' => [$instance],
);

$testedMethods = array();
foreach ($methodsAndParams as $methodName => $params) {

	if ($reflectionClass->hasMethod($methodName) == false) {
		continue;
	}

    try {
        call_user_func_array([$instance, $methodName], $params);
        echo "failed to throw an exception.\n";
    }
    catch (ImagickPixelException $ipe) {
        if (strpos($ipe->getMessage(), "Can not process empty ImagickPixel object") === false) {
            echo "Incorrect message: " . $ipe->getMessage() . "\n";
        }
    }

    $testedMethods[] = strtolower($methodName);
}


// pretend we tested these.
$testedMethods[] = '__construct';
$testedMethods[] = 'clone';

foreach ($methods as $method) {
	$allMethods[] = strtolower($method->getName());
}

// Have we tested all but __construct
$missedMethods = array_diff($allMethods, $testedMethods);

if (count($missedMethods) !== 0) {
	echo "We didn't test all of the ImagickPixel methods\n";
	var_dump($missedMethods);
}

echo "Ok"

?>
--EXPECTF--

Ok