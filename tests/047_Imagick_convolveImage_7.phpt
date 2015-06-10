--TEST--
Test Imagick, convolveImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$bias = 0.5;
$kernelMatrix = array (
    array (-1, -1, -1),
    array (-1, 8, -1),
    array( -1, -1, -1),
);

function convolveImage($bias, $kernelMatrix) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $kernel = ImagickKernel::fromMatrix($kernelMatrix);
    $imagick->convolveImage($kernel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

convolveImage($bias, $kernelMatrix) ;
echo "Ok";
?>
--EXPECTF--
Ok