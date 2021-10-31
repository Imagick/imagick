<?php

declare(strict_types = 1);

// This was used to figure out what parameters were called during the
// update to PHP 8, as that's the time when named params became a thing,
// and so parameter names became part of the API, mostly to spite Marco.
$classes = [
    'Imagick',
    'ImagickDraw',
    'ImagickPixel',
    'ImagickKernel'
];


foreach ($classes as $class) {
    echo "Checking class = $class \n";

    $nameCounts = [];
    $classReflection = new ReflectionClass($class);
    $classMethods = $classReflection->getMethods();

    foreach ($classMethods as $imagickMethod) {
        $parameters = $imagickMethod->getParameters();
        foreach ($parameters as $parameter) {
            $name = $parameter->getName();
            $nameCounts[$name] = ($nameCounts[$name] ?? 0) + 1;
        }
    }

    arsort($nameCounts);

    foreach ($nameCounts as $name => $count) {
        echo "$name: $count \n";
    }

    echo "\n\n";
}
