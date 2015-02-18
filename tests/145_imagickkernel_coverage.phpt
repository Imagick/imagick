--TEST--
ImagickKernel::fromMatrix test
--SKIPIF--
<?php 

$imageMagickRequiredVersion = 680;
require_once(dirname(__FILE__) . '/skipif.inc');
 
?>
--FILE--
<?php


$kernel = [
	[1, 0, -1],
	[1, 0, -1],
	[1, 0, -1],
];

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


$testValues = [];

$testValues[] = array(); //Should fail, not a 2d matrix

$testValues[] = array(0, 1, 1); //Should fail, not a 2d matrix

//Should fail, matrix needs to have same number columns in each row
$testValues[] = array(
	array(1, 0, 1),
	array(1, 0, 1),
	array(1, 0),
);

//Should fail, value instead of second row
$testValues[] = array(
	array(0, 1),
	1
);

//Should fail, value instead of second row
$testValues[] = array(
	array(0, 1),
	array(0, new StdClass),
);

foreach ($testValues as $testValue) {
	try {
		$kernel = ImagickKernel::fromMatrix($testValue);
		echo "Failed to throw exception".PHP_EOL;
	}
	catch(ImagickKernelException $e) {
		echo $e->getMessage().PHP_EOL;
	}
}


try {
	$kernel = ImagickKernel::fromBuiltin(\Imagick::KERNEL_DIAMOND, "CestNestPasUneKernel");
	echo "builtIn OK".PHP_EOL;
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
echo "Adding unity kernel ok".PHP_EOL;

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
echo "getMatrix OK".PHP_EOL;

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
echo "Scaling OK".PHP_EOL;

//Test single line kernel missing origin exception
$matrixIn = array(
	array(1, 0),
);
try {
	$kernel = ImagickKernel::fromMatrix($matrixIn);
	echo "No exception thrown?";
}
catch(ImagickKernelException $e) {
	echo $e->getMessage().PHP_EOL;
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

echo "Complete".PHP_EOL;
?>
--EXPECTF--
Cannot create kernel, matrix is empty.
Values must be matrix, with the same number of columns in each row.
Values must be matrix, with the same number of columns in each row.
Values must be matrix, with the same number of columns in each row.
Only numbers or false are valid values in a kernel matrix.
builtIn OK
Adding unity kernel ok
getMatrix OK
Scaling OK
For kernels with even numbered rows or columns, the origin position must be specified.
Complete