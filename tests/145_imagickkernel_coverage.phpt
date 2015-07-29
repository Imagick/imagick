--TEST--
ImagickKernel::fromMatrix test
--SKIPIF--
<?php 

$imageMagickRequiredVersion = 0x680;
require_once(dirname(__FILE__) . '/skipif.inc');
 
?>
--FILE--
<?php


$kernel = array(
	array(1, 0, -1),
	array(1, 0, -1),
	array(1, 0, -1),
);

$kernel = ImagickKernel::fromMatrix($kernel);
$kernel->scale(1, \Imagick::NORMALIZE_KERNEL_VALUE);
$kernel->addUnityKernel(0.50);
$imagick = new \Imagick();
$imagick->newPseudoImage(640, 480, "magick:logo");
$imagick->morphology(
	\Imagick::MORPHOLOGY_CONVOLVE,
	1,
	$kernel
);


$tests = array();

$tests[] = array( 
	"Cannot create kernel, matrix is empty.",
	array()
);

$tests[] = array(
	"Values must be matrix, with the same number of columns in each row.",
	array(0, 1, 1)
);

//Should fail, matrix needs to have same number columns in each row
$tests[] = array(
	"Values must be matrix, with the same number of columns in each row.",
	array(
		array(1, 0, 1),
		array(1, 0, 1),
		array(1, 0),
	)
);

//Should fail, value instead of second row
$tests[] = array(
	"Values must be matrix, with the same number of columns in each row.",
	array(
		array(0, 1),
		1
	)
);

//Should fail, value instead of second row
$tests[] = array( 
	"Only numbers or false are valid values in a kernel matrix.",
	array(
		array(0, 1),
		array(0, new StdClass),
	)
);

$tests[] = array(
	"For kernels with even numbered rows or columns, the origin position must be specified.",
	array(
		array(1, 0),
	),
);


foreach ($tests as $test) {

	list($expectedMessage, $testValue) = $test;

	try {
		$kernel = ImagickKernel::fromMatrix($testValue);
		echo "Failed to throw exception".PHP_EOL;
	}
	catch(ImagickKernelException $e) {
		if ($e->getMessage() != $expectedMessage) {
			echo "Unexpected message ".$e->getMessage()." for test:".PHP_EOL;
			var_dump($test);
		}
	}
}


try {
	$kernel = ImagickKernel::fromBuiltin(\Imagick::KERNEL_DIAMOND, "CestNestPasUneKernel");
	//echo "builtIn OK".PHP_EOL;
}
catch(Exception $e) {
	echo "Unexpected exception: ".$e->getMessage().PHP_EOL;
}


//Test adding unity kernel works.
$matrix1 = array(
	array(1, 1, 1),
	array(0, 0, 0),
	array(-1, -1, -1),
);
$kernel = ImagickKernel::fromMatrix($matrix1);
$kernel->addUnityKernel(0.5);
$matrix = $kernel->getMatrix();

if ($matrix[1][1] != 0.5) {
	echo "center point should be 0.5 but is actually ".$matrix[1][1].PHP_EOL;
	var_dump($matrix);
}
//echo "Adding unity kernel ok".PHP_EOL;

// Test adding kernel works and you can get the values back
$matrix1 = array(
	array(1, 1),
	array(0, 0),
);
$matrix2 = array(
	array(0, 0),
	array(1, 1),
);
$kernel1 = ImagickKernel::fromMatrix($matrix1, array(0, 0));
$kernel2 = ImagickKernel::fromMatrix($matrix2, array(0, 0));
$kernel1->addKernel($kernel2);

$kernelList = $kernel1->separate();
if (count($kernelList) != 2) {
	echo "Incorrect number of matrixes returned.";
}
else {
	if ($kernelList[0]->getMatrix() != $matrix1) {
		echo "Matrix 1 does not match".PHP_EOL;
		var_dump($kernelList[0]);
	}
	if ($kernelList[1]->getMatrix() != $matrix2) {
		echo "Matrix 2 does not match".PHP_EOL;
		var_dump($kernelList[1]);
	}
}

//Test Scaling
$matrixIn = array(
	array(-1, 0, -1),
	array( 0, 8,  0),
	array(-1, 0, -1),
);
$kernel = ImagickKernel::fromMatrix($matrixIn);
$kernel->scale(1, \Imagick::NORMALIZE_KERNEL_VALUE);
$matrixOut = $kernel->getMatrix();

if ($matrixOut[1][1] != 2) {
	echo "Matrix was not normalised correctly.";
	var_dump($matrixOut);
}



//Test single line kernel works
$matrixIn = array(
	array(1, 0),
);
$kernel = ImagickKernel::fromMatrix($matrixIn, array(1, 0));
if ($kernel->getMatrix() != $matrixIn) {
	echo "Values do not match for 'Test single line kernel works'".PHP_EOL;
}

//Test even sized kernel works
$matrixIn = array(
	array(-1, 0),
	array( 0, 1)
);
$kernel = ImagickKernel::fromMatrix($matrixIn, array(0, 1));
if ($kernel->getMatrix() != $matrixIn) {
	echo "Values do not match for 'Test even sized kernel works'".PHP_EOL;
}

//Test 'wrong' order matrix is converted correctly.
$matrix = array(
	array(0.0, 1.0),
	array(0.5, false)
);
 
$outOfOrderMatrix = array();
$outOfOrderMatrix[1][1] = $matrix[1][1];
$outOfOrderMatrix[1][0] = $matrix[1][0];
$outOfOrderMatrix[0][1] = $matrix[0][1];
$outOfOrderMatrix[0][0] = $matrix[0][0];

$kernel = ImagickKernel::fromMatrix($outOfOrderMatrix, array(0, 0));
$kernelMatrix = $kernel->getMatrix();
if ($kernelMatrix !== $matrix) {
	echo "Kernel generated from 'out of order' matrix is incorrect.".PHP_EOL;
	var_dump($matrix);
	echo "vs".PHP_EOL;
	var_dump($kernelMatrix);
}

echo "Complete".PHP_EOL;
?>
--EXPECTF--
Complete
