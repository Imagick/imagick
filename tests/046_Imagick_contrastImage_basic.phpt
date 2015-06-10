--TEST--
Test Imagick, contrastImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$contrastType = 1;

function contrastImage($contrastType) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    if ($contrastType != 2) {
        $imagick->contrastImage($contrastType);
    }

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

contrastImage($contrastType) ;
echo "Ok";
?>
--EXPECTF--
Ok