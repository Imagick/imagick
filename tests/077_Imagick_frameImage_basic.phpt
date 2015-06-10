--TEST--
Test Imagick, frameImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$color = 'rgb(127, 127, 127)';
$width = 5;
$height = 5;
$innerBevel = 3;
$outerBevel = 3;

function frameImage($color, $width, $height, $innerBevel, $outerBevel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

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
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

frameImage($color, $width, $height, $innerBevel, $outerBevel) ;
echo "Ok";
?>
--EXPECTF--
Ok