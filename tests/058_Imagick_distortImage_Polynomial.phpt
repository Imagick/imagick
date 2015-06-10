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


// Order     X1,Y1 I1,J1     X2,Y2 I2,J2     X3,Y3 I3,J3     X4,Y4 I4,J4 . . . .
// The 'Order' argument is usually an integer from '1' onward, though a special value
// of '1.5' can also be used. This defines the 'order' or complexity of the 2-dimensional
// mathematical equation (using both 'x' and 'y') , that will be applied.
// For example an order '1' polynomial will fit a equation of the form...
// Xd = 	 C2x*Xs + C1x*Ys + C0x	  ,      	Yd = 	 C2y*Xs + C1y*Ys + C0y 
// See also http://www.imagemagick.org/Usage/distorts/#polynomial

        $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
        $points = array(
            1.5,   //Order 1.5 = special
            0, 0, 26, 0,   
            128,0,  114,23,  
            128,128,  128,100,
            0,128,  0,123
        );
        $imagick->setimagebackgroundcolor("#fad888");
        $imagick->setImageVirtualPixelMethod( \Imagick::VIRTUALPIXELMETHOD_BACKGROUND);
        $imagick->distortImage(\Imagick::DISTORTION_POLYNOMIAL, $points, TRUE);
    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "Ok";
?>
--EXPECTF--
Ok