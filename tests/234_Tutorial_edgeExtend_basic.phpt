--TEST--
Test Tutorial, edgeExtend
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$virtualPixelType = 5;

function edgeExtend($virtualPixelType) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->setImageVirtualPixelMethod($virtualPixelType);

    $imagick->scaleimage(400, 300, true);

    $imagick->setbackgroundcolor('pink');
   
    $desiredWidth = 600;
    $originalWidth = $imagick->getImageWidth();

    //Make the image be the desired width.
    $imagick->sampleimage($desiredWidth, $imagick->getImageHeight());

    //Now scale, rotate, translate (aka affine project) it
    //to be how you want
    $points = array(//The x scaling factor is 0.5 when the desired width is double
        //the source width
        ($originalWidth / $desiredWidth), 0, //Don't scale vertically
        0, 1, //Offset the image so that it's in the centre
        ($desiredWidth - $originalWidth) / 2, 0);

    $imagick->distortImage(\Imagick::DISTORTION_AFFINEPROJECTION, $points, false);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

//Fyi it may be easier to think of the affine transform by 
//how it works for a rotation:
//$affineRotate = array(
//    "sx" => cos($angle),
//    "sy" => cos($angle),
//    "rx" => sin($angle),
//    "ry" => -sin($angle),
//    "tx" => 0,
//    "ty" => 0,
//);
}

edgeExtend($virtualPixelType) ;
echo "Ok";
?>
--EXPECTF--
Ok