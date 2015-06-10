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
            0.9,0.3,
            -0.2,0.7,
            20,15
        );
        $imagick->setimagebackgroundcolor("#fad888");
        $imagick->setImageVirtualPixelMethod( \Imagick::VIRTUALPIXELMETHOD_BACKGROUND );
        $imagick->distortImage( \Imagick::DISTORTION_AFFINEPROJECTION, $points, TRUE );
        header( "Content-Type: image/jpeg" );
    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "Ok";
?>
--EXPECTF--
Ok