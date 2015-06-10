--TEST--
Test Imagick, sigmoidalContrastImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$sharpening = 1;
$midpoint = 4;
$sigmoidalContrast = 0.5;

function sigmoidalContrastImage($sharpening, $midpoint, $sigmoidalContrast) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    //Need some stereo image to work with.
    $imagick->sigmoidalcontrastimage(
        $sharpening, //sharpen 
        $midpoint,
        $sigmoidalContrast * \Imagick::getQuantum()
    );
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

sigmoidalContrastImage($sharpening, $midpoint, $sigmoidalContrast) ;
echo "Ok";
?>
--EXPECTF--
Ok