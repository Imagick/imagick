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

        //X-of-destination = (sx*xs + ry+ys +tx) / (px*xs + py*ys +1)
        //Y-of-destination = (rx*xs + sy+ys +ty) / (px*xs + py*ys +1)

        // sx   ry   tx
        // rx   sy   ty
        // px   py
        
        $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
        $points = array(
            1.945622, 0.071451, 
            -12.187838, 0.799032, 
            1.276214, -24.470275, 0.006258, 0.000715
        );
        $imagick->setimagebackgroundcolor("#fad888");
        $imagick->setImageVirtualPixelMethod( \Imagick::VIRTUALPIXELMETHOD_BACKGROUND);
        $imagick->distortImage(\Imagick::DISTORTION_PERSPECTIVEPROJECTION, $points, TRUE);
    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "Ok";
?>
--EXPECTF--
Ok