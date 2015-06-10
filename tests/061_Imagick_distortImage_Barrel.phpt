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


// The arguments needed for the 'Barrel' distort method. Generally you supply
// 3 or 4 values only...
// A   B   C   [ D   [ X , Y ] ]
// The optional X,Y arguments provide an optional 'center' for the radial distortion,
// otherwise it defaults to the exact center of the image given (regardless of its virtual offset).
// The coefficients are designed so that if all four A to D values, add up to '1.0', the minimal
// width/height of the image will not change. For this reason if D (which controls the overall
// scaling of the image) is not supplied it will be set so all four values do add up to '1.0'.
    
        $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
     
        $points = array(
            //0.2, 0.0, 0.0, 1.0
            0.4, 0.6, 0.0, 1.0
        );

        $imagick->setimagebackgroundcolor("#fad888");
        $imagick->setImageVirtualPixelMethod( \Imagick::VIRTUALPIXELMETHOD_EDGE);
        $imagick->distortImage(\Imagick::DISTORTION_BARREL, $points, TRUE);
    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "Ok";
?>
--EXPECTF--
Ok