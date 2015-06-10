--TEST--
Test Imagick, setOption
--SKIPIF--
<?php
//Double-free issue in ImageMagick makes test un-runnable before 690
$imageMagickRequiredVersion=0x690;
require_once(dirname(__FILE__) . '/skipif.inc'); 
?>
--FILE--
<?php

$extent = '10kb';

function renderJPG($extent) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->setImageFormat('jpg');
    $imagick->setOption('jpeg:extent', $extent);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

renderJPG($extent) ;
echo "Ok";
?>
--EXPECTF--
Ok