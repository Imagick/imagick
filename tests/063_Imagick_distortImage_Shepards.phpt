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

        //The control points move points in the image in a taffy like motion
        $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

        $points = array(

            //Setup some control points that don't move
            5 * $imagick->getImageWidth() / 100, 5 * $imagick->getImageHeight() / 100,
            5 * $imagick->getImageWidth() / 100, 5 * $imagick->getImageHeight() / 100,
            
            5 * $imagick->getImageWidth() / 100, 95 * $imagick->getImageHeight() / 100,
            5 * $imagick->getImageWidth() / 100, 95 * $imagick->getImageHeight() / 100,

            95 * $imagick->getImageWidth() / 100, 95 * $imagick->getImageHeight() / 100,
            95 * $imagick->getImageWidth() / 100, 95 * $imagick->getImageHeight() / 100,

            5 * $imagick->getImageWidth() / 100, 5 * $imagick->getImageHeight() / 100,
            95 * $imagick->getImageWidth() / 100, 95 * $imagick->getImageHeight() / 100,
//            //Move the centre of the image down and to the right
//            50 * $imagick->getImageWidth() / 100, 50 * $imagick->getImageHeight() / 100,
//            60 * $imagick->getImageWidth() / 100, 60 * $imagick->getImageHeight() / 100,
//
//            //Move a point near the top-right of the image down and to the left and down
//            90 * $imagick->getImageWidth(), 10 * $imagick->getImageHeight(),
//            80 * $imagick->getImageWidth(), 15 * $imagick->getImageHeight(),  
        );

        $imagick->setimagebackgroundcolor("#fad888");
        $imagick->setImageVirtualPixelMethod( \Imagick::VIRTUALPIXELMETHOD_EDGE);
        $imagick->distortImage(\Imagick::DISTORTION_SHEPARDS, $points, TRUE);
    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "Ok";
?>
--EXPECTF--
Ok