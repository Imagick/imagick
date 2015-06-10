--TEST--
Test Imagick, uniqueImageColors
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function uniqueImageColors() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    //Reduce the image to 256 colours nicely.
    $imagick->quantizeImage(256, \Imagick::COLORSPACE_YIQ, 0, false, false);
    $imagick->uniqueImageColors();
    $imagick->scaleimage($imagick->getImageWidth(), $imagick->getImageHeight() * 20);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

uniqueImageColors() ;
echo "Ok";
?>
--EXPECTF--
Ok