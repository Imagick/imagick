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
            1.5, # scale 150%
            150 # rotate
        );
        $imagick->setimagebackgroundcolor("#fad888");
        $imagick->setImageVirtualPixelMethod( \Imagick::VIRTUALPIXELMETHOD_BACKGROUND );
        $imagick->distortImage( \Imagick::DISTORTION_SCALEROTATETRANSLATE, $points, TRUE );
    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "Ok";
?>
--EXPECTF--
Ok