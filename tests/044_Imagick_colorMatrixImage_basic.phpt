--TEST--
Test Imagick, colorMatrixImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
checkFormatPresent('png');
?>
--FILE--
<?php

$colorMatrix = array (
  0 => 1.5,
  1 => 0,
  2 => 0,
  3 => 0,
  4 => -0.157,
  5 => 0,
  6 => 1,
  7 => 0.5,
  8 => 0,
  9 => -0.157,
  10 => 0,
  11 => 0,
  12 => 0.5,
  13 => 0,
  14 => 0.5,
  15 => 0,
  16 => 0,
  17 => 0,
  18 => 1,
  19 => 0,
  20 => 0,
  21 => 0,
  22 => 0,
  23 => 0,
  24 => 1,
);

function colorMatrixImage($colorMatrix) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    //$imagick->setImageOpacity(1);

    //A color matrix should look like:
    //    $colorMatrix = [
    //        1.5, 0.0, 0.0, 0.0, 0.0, -0.157,
    //        0.0, 1.0, 0.5, 0.0, 0.0, -0.157,
    //        0.0, 0.0, 1.5, 0.0, 0.0, -0.157,
    //        0.0, 0.0, 0.0, 1.0, 0.0,  0.0,
    //        0.0, 0.0, 0.0, 0.0, 1.0,  0.0,
    //        0.0, 0.0, 0.0, 0.0, 0.0,  1.0
    //    ];

    $background = new \Imagick();
    $background->newPseudoImage($imagick->getImageWidth(), $imagick->getImageHeight(),  "pattern:checkerboard");

    $background->setImageFormat('png');

    $imagick->setImageFormat('png');
    $imagick->colorMatrixImage($colorMatrix);
    
    $background->compositeImage($imagick, \Imagick::COMPOSITE_ATOP, 0, 0);

    $bytes = $background->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

colorMatrixImage($colorMatrix) ;
echo "Ok";
?>
--EXPECTF--
Ok