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

        //v6.4.2-6
        $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
        $points = array(
            0
        );     

        $imagick->setimagebackgroundcolor("#fad888");
        $imagick->setImageVirtualPixelMethod( \Imagick::VIRTUALPIXELMETHOD_BACKGROUND);
        $imagick->distortImage(\Imagick::DISTORTION_DEPOLAR, $points, TRUE);
    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "Ok";
?>
--EXPECTF--
Ok