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

//  Rsrc = r / ( A*r3 + B*r2 + C*r + D )
// This equation does NOT produce the 'reverse' the 'Barrel' distortion.
// You can NOT use it to 'undo' the previous distortion.

        $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

        $points = array(
            //0.2, 0.0, 0.0, 1.0
            0.2, 0.1, 0.0, 1.0
        );

        $imagick->setimagebackgroundcolor("#fad888");
        $imagick->setImageVirtualPixelMethod( \Imagick::VIRTUALPIXELMETHOD_EDGE);
        $imagick->distortImage(\Imagick::DISTORTION_BARRELINVERSE, $points, TRUE);
    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "Ok";
?>
--EXPECTF--
Ok