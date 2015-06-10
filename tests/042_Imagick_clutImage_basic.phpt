--TEST--
Test Imagick, clutImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc'); 
?>
--FILE--
<?php


function clutImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    //$imagick->quantizeImage(16, \Imagick::COLORSPACE_YIQ, 8, true, false);
    
    $clutImagick = new \Imagick();
    $clutImagick->newPseudoImage(640, 480, "magick:NETSCAPE");
    $imagick->clutImage($clutImagick);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

clutImage() ;
echo "Ok";
?>
--EXPECTF--
Ok