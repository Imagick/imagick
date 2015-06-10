--TEST--
Test Imagick, medianFilterImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('medianFilterImage'));
?>
--FILE--
<?php

$radius = 5;

function medianFilterImage($radius) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    @$imagick->medianFilterImage($radius);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

medianFilterImage($radius) ;
echo "Ok";
?>
--EXPECTF--
Ok