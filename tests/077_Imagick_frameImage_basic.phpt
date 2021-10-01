--TEST--
Test Imagick, frameImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

//$color = 'rgb(127, 127, 127)';

$color = 'rgba(255,0,255,50)';
//$color = 'rgb(255,0,255)';
$width = 40;
$height = 40;
$innerBevel = 10;
$outerBevel = 10;

function frameImage($color, $width, $height, $innerBevel, $outerBevel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->setImageFormat('png');

    $width = $width + $innerBevel + $outerBevel;
    $height = $height + $innerBevel + $outerBevel;

    $imagick->frameimage(
        $color,
        $width,
        $height,
        $innerBevel,
        $outerBevel
    );
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) {
        echo "Failed to generate image.";
    }
    return $imagick;
}

$imagick = frameImage($color, $width, $height, $innerBevel, $outerBevel) ;
// $imagick->writeImage("frame_image.png");

echo "Ok";
?>
--EXPECTF--
Ok