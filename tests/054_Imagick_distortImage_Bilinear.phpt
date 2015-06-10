--TEST--
Test Imagick, distortImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$distortion = 1;

        $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
        $points = array(
            0,0, 25,25, # top left  
            176,0, 126,0, # top right
            0,135, 0,105, # bottom right 
            176,135, 176,135 # bottum left
        );
        $imagick->setImageBackgroundColor("#fad888");
        $imagick->setImageVirtualPixelMethod( \Imagick::VIRTUALPIXELMETHOD_BACKGROUND );
        $imagick->distortImage( \Imagick::DISTORTION_BILINEAR, $points, TRUE );
        header( "Content-Type: image/jpeg" );
    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "Ok";
?>
--EXPECTF--
Ok