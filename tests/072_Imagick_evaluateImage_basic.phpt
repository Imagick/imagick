--TEST--
Test Imagick, evaluateImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
checkFormatPresent('png');
?>
--FILE--
<?php

$evaluateType = 1;
$firstTerm = 0.5;
$gradientStartColor = 'black';
$gradientEndColor = 'white';

$evaluateTypes = array(
	\Imagick::EVALUATE_ADD,
	\Imagick::EVALUATE_AND,
	\Imagick::EVALUATE_MAX,
	\Imagick::EVALUATE_MIN,
	\Imagick::EVALUATE_OR,
	\Imagick::EVALUATE_SET,
	\Imagick::EVALUATE_SUBTRACT,
	\Imagick::EVALUATE_XOR,
	\Imagick::EVALUATE_THRESHOLD,
	\Imagick::EVALUATE_THRESHOLDBLACK,
	\Imagick::EVALUATE_THRESHOLDWHITE,
	\Imagick::EVALUATE_ADDMODULUS,

	\Imagick::EVALUATE_DIVIDE,
	\Imagick::EVALUATE_MULTIPLY,
	\Imagick::EVALUATE_RIGHTSHIFT,
	\Imagick::EVALUATE_LEFTSHIFT,
	\Imagick::EVALUATE_POW,
	\Imagick::EVALUATE_LOG,
	\Imagick::EVALUATE_GAUSSIANNOISE,
	\Imagick::EVALUATE_IMPULSENOISE,
	\Imagick::EVALUATE_LAPLACIANNOISE,
	\Imagick::EVALUATE_MULTIPLICATIVENOISE,
	\Imagick::EVALUATE_POISSONNOISE,
	\Imagick::EVALUATE_UNIFORMNOISE,
	\Imagick::EVALUATE_COSINE,
	\Imagick::EVALUATE_SINE,
);



function evaluateImage($evaluateType, $firstTerm, $gradientStartColor, $gradientEndColor) {
    $imagick = new \Imagick();
    $size = 400;
    $imagick->newPseudoImage(
        $size,
        $size,
        "gradient:$gradientStartColor-$gradientEndColor"
    );
    
    $quantumScaledTypes = array(
        \Imagick::EVALUATE_ADD,
        \Imagick::EVALUATE_AND,
        \Imagick::EVALUATE_MAX,
        \Imagick::EVALUATE_MIN,
        \Imagick::EVALUATE_OR,
        \Imagick::EVALUATE_SET,
        \Imagick::EVALUATE_SUBTRACT,
        \Imagick::EVALUATE_XOR,
        \Imagick::EVALUATE_THRESHOLD,
        \Imagick::EVALUATE_THRESHOLDBLACK,
        \Imagick::EVALUATE_THRESHOLDWHITE,
        \Imagick::EVALUATE_ADDMODULUS,
    );

    $unscaledTypes = array(
        \Imagick::EVALUATE_DIVIDE,
        \Imagick::EVALUATE_MULTIPLY,
        \Imagick::EVALUATE_RIGHTSHIFT,
        \Imagick::EVALUATE_LEFTSHIFT,
        \Imagick::EVALUATE_POW,
        \Imagick::EVALUATE_LOG,
        \Imagick::EVALUATE_GAUSSIANNOISE,
        \Imagick::EVALUATE_IMPULSENOISE,
        \Imagick::EVALUATE_LAPLACIANNOISE,
        \Imagick::EVALUATE_MULTIPLICATIVENOISE,
        \Imagick::EVALUATE_POISSONNOISE,
        \Imagick::EVALUATE_UNIFORMNOISE,
        \Imagick::EVALUATE_COSINE,
        \Imagick::EVALUATE_SINE,
    );

    if (in_array($evaluateType, $unscaledTypes)) {
        $imagick->evaluateimage($evaluateType, $firstTerm);
    }
    else if (in_array($evaluateType, $quantumScaledTypes)) {
        $imagick->evaluateimage($evaluateType, $firstTerm * \Imagick::getQuantum());
    }
    else {
        throw new \Exception("Evaluation type $evaluateType is not listed as either scaled or unscaled");
    }

    $imagick->setimageformat('png');
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

foreach ($evaluateTypes as $evaluateType) {
	evaluateImage($evaluateType, $firstTerm, $gradientStartColor, $gradientEndColor) ;
}
echo "Ok";
?>
--EXPECTF--
Ok