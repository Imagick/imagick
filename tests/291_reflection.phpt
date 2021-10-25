--TEST--
Test that reflection can get default values
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$class_list = [
    'Imagick',
    'ImagickDraw',
    'ImagickPixel',
    'ImagickPixelIterator',
];

if (class_exists('ImagickKernel') === true) {
    $class_list[] = 'ImagickKernel';
}

foreach ($class_list as $class) {
    $rc = new ReflectionClass($class);
    foreach ($rc->getMethods() as $reflectionMethod) {
        $parameters = $reflectionMethod->getParameters();
        foreach ($parameters as $parameter) {
            if ($parameter->isDefaultValueAvailable() !== true) {
                continue;
            }

            try {
                $value = $parameter->getDefaultValue();
            }
            catch (ReflectionException $re) {
                $method_name = $reflectionMethod->getName();
                echo "Exception for $class::$method_name : " . $re->getMessage() . "\n";
            }
        }
    }
}

echo "Ok";
?>
--EXPECTF--
Ok

