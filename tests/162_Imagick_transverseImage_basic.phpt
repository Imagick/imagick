--TEST--
Test Imagick, transverseImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function transverseImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->transverseImage();
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

transverseImage() ;
echo "Ok";
?>
--EXPECTF--
Ok