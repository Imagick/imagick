--TEST--
ImagickKernel::fromMatrix exceptions
--SKIPIF--
<?php 

$imageMagickRequiredVersion = 0x680;
require_once(dirname(__FILE__) . '/skipif.inc');
 
?>
--FILE--
<?php


$kernelArray = array(
	array(1, 0, -1),
	array(1, 0, -1),
	array(1, 0, -1),
);

$validOrigins = [
    [0, 0],
    [2, 0],
    [0, 2],
    [2, 2],
    [1, 2]
];

$invalidOrigins = [
    [-1, 0],
    [3, 0],
    [0, 3],
    [3, 3],
    [1, PHP_INT_MAX - 10],
];


foreach ($validOrigins as $validOrigin) {
    try {
        $kernel = ImagickKernel::fromMatrix($kernelArray, $validOrigin);
    }
    catch (\Exception $e) {
        echo "unexpected exception: " . $e->getMessage();
    }
}

foreach ($invalidOrigins as $invalidOrigin) {
    try {
        $kernel = ImagickKernel::fromMatrix($kernelArray, $invalidOrigin);
        echo "Exception wasn't thrown for case: \n";
        var_dump($invalidOrigin);
    }
    catch (\ImagickKernelException $e) {
        $message = $e->getMessage();
        if (strpos($message, "origin_y for matrix is outside bounds of rows") === 0) {
            // this is fine.
        }
        else if (strpos($message, "origin_x for matrix is outside bounds of columns") === 0) {
            // this is fine.
        }
        else {
            echo "Unexpected message: " . $message . "\n";
        }
    }
}

$flatKernelArray = array(
	array(1, 0, -2, 0, 1),
);

try {
    $kernel = ImagickKernel::fromMatrix($flatKernelArray, [1, 4]);
    echo "Exception wasn't thrown for case: \n";
    var_dump($invalidOrigin);
}
catch (\ImagickKernelException $e) {
    $message = $e->getMessage();
    if (strpos($message, "origin_y for matrix is outside bounds of rows") === 0) {
        // this is fine.
    }
    else {
        echo "Unexpected message: " . $message . "\n";
    }
}


$tallKernelArray = array(
	array(1),
	array(0),
	array(-2),
	array(0),
	array(1),
);


try {
    $kernel = ImagickKernel::fromMatrix($tallKernelArray, [4, 1]);
    echo "Exception wasn't thrown for case: \n";
    var_dump($invalidOrigin);
}
catch (\ImagickKernelException $e) {
    $message = $e->getMessage();
    if (strpos($message, "origin_x for matrix is outside bounds of columns") === 0) {
        // this is fine.
    }
    else {
        echo "Unexpected message: " . $message . "\n";
    }
}



echo "Complete".PHP_EOL;
?>
--EXPECTF--
Complete
