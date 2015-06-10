--TEST--
Test Imagick, trimImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$color = 'rgb(39, 194, 255)';
$fuzz = 0.1;

function trimImage($color, $fuzz) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    
    $imagick->borderImage($color, 10, 10);
    $imagick->trimImage($fuzz * \Imagick::getQuantum());

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

trimImage($color, $fuzz) ;
echo "Ok";
?>
--EXPECTF--
Ok