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

        //Make some text arc around the center of it's image
//        convert logo: -resize x150 -gravity NorthEast -crop 100x100+10+0! \
//        \( -background none label:'IM Examples' \
//        -virtual-pixel Background +distort Arc '270 50 20' \
//        -repage +75+21\! \)  -flatten  arc_overlay.jpg

        $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
        $degrees = array( 180 );
        $imagick->setimagebackgroundcolor("#fad888");
        $imagick->setImageVirtualPixelMethod( \Imagick::VIRTUALPIXELMETHOD_BACKGROUND );
        $imagick->distortImage( \Imagick::DISTORTION_ARC, $degrees, TRUE );
        header( "Content-Type: image/jpeg" );
    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "Ok";
?>
--EXPECTF--
Ok