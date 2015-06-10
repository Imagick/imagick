--TEST--
Test Tutorial, levelizeImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$blackPoint = 50;
$whitePoint = 100;
$gamma = 2.2;

function levelizeImage($blackPoint, $gamma,  $whitePoint) {
    $imagick = new \Imagick();
    $imagick->newPseudoimage(300, 300, 'gradient:black-white');
    $maxQuantum = $imagick->getQuantum();
    $imagick->evaluateimage(\Imagick::EVALUATE_POW, 1 / $gamma);
    
    //Adjust the scale from black to white to the new 'distance' between black and white
    $imagick->evaluateimage(\Imagick::EVALUATE_MULTIPLY, ($whitePoint - $blackPoint) / 100 );

    //Add move the black point to it's new value
    $imagick->evaluateimage(\Imagick::EVALUATE_ADD, ($blackPoint / 100) * $maxQuantum);
    $imagick->setFormat("png");

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

levelizeImage($blackPoint, $gamma,  $whitePoint) ;
echo "Ok";
?>
--EXPECTF--
Ok