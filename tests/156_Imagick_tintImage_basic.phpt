--TEST--
Test Imagick, tintImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$r = 100;
$g = 50;
$b = 100;
$a = 100;

function tintImage($r, $g, $b, $a) {
    $a = $a / 100;

    $imagick = new \Imagick();
    $imagick->newPseudoImage(400, 400, 'gradient:black-white');

    $tint = new \ImagickPixel("rgb($r, $g, $b)");
    $opacity = new \ImagickPixel("rgb(128, 128, 128, $a)");
    $imagick->tintImage($tint, $opacity);
    $imagick->setImageFormat('png');
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

tintImage($r, $g, $b, $a) ;
echo "Ok";
?>
--EXPECTF--
Ok