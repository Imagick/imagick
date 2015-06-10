--TEST--
Test Imagick, reduceNoiseImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('reduceNoiseImage'));
?>
--FILE--
<?php

$reduceNoise = 5;

function reduceNoiseImage($reduceNoise) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    @$imagick->reduceNoiseImage($reduceNoise);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

reduceNoiseImage($reduceNoise) ;
echo "Ok";
?>
--EXPECTF--
Ok