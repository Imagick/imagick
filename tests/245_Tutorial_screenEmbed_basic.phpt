--TEST--
Test Tutorial, screenEmbed
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function screenEmbed() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

    $overlay = new \Imagick();
    $overlay->newPseudoImage(640, 480, "magick:logo");

    $overlay->setImageVirtualPixelMethod(\Imagick::VIRTUALPIXELMETHOD_TRANSPARENT);

    $width = $overlay->getImageWidth();
    $height = $overlay->getImageHeight();

    $offset = 332.9;

    $points = array(    
        0, 0, 364 - $offset, 51, 
        $width, 0, 473.4 - $offset, 23, 
        0, $height, 433.5 - $offset, 182, 
        $width, $height, 523 - $offset, 119.4
    );

    $overlay->modulateImage(97, 100, 0);
    $overlay->distortImage(\Imagick::DISTORTION_PERSPECTIVE, $points, true);

    $imagick->compositeImage($overlay, \Imagick::COMPOSITE_OVER, 364.5 - $offset, 23.5);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

screenEmbed() ;
echo "Ok";
?>
--EXPECTF--
Ok