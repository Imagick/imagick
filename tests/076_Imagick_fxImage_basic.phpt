--TEST--
Test Imagick, fxImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
checkFormatPresent('png');
?>
--FILE--
<?php


function fxImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(200, 200, "xc:white");

    $fx = 'xx=i-w/2; yy=j-h/2; rr=hypot(xx,yy); (.5-rr/140)*1.2+.5';
    $fxImage = $imagick->fxImage($fx);

    $fxImage->setimageformat('png');
    $bytes = $fxImage->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

fxImage() ;
echo "Ok";
?>
--EXPECTF--
Ok