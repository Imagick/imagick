--TEST--
Test Imagick, levelImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$blackPoint = 50;
$whitePoint = 100;
$gamma = 2.2;

function levelImage($blackPoint, $gamma, $whitePoint) {
    $imagick = new \Imagick();
    $imagick->newPseudoimage(500, 500, 'gradient:black-white');

    $imagick->setFormat('png');
    $quantum = $imagick->getQuantum();
    $imagick->levelImage($blackPoint / 100 , $gamma, $quantum * $whitePoint / 100);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

levelImage($blackPoint, $gamma, $whitePoint) ;
echo "Ok";
?>
--EXPECTF--
Ok