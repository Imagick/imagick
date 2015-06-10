--TEST--
Test Imagick, setCompressionQuality
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$quality = 15;

function setCompressionQuality($quality) {

    $backgroundImagick = new \Imagick();
    $backgroundImagick->newPseudoImage(640, 480, "magick:logo");
    
    $imagick = new \Imagick();
    $imagick->setCompressionQuality($quality);
    $imagick->newPseudoImage(
        $backgroundImagick->getImageWidth(),
        $backgroundImagick->getImageHeight(),
        'canvas:white'
    );

    $imagick->compositeImage(
        $backgroundImagick,
        \Imagick::COMPOSITE_ATOP,
        0,
        0
    );
    
    $imagick->setFormat("jpg");    
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setCompressionQuality($quality) ;
echo "Ok";
?>
--EXPECTF--
Ok