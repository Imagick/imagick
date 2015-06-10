--TEST--
Test Imagick, transformImageColorspace
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$colorSpace = 1;
$channel = Imagick::CHANNEL_DEFAULT;

function transformImageColorspace($colorSpace, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->transformimagecolorspace($colorSpace);
    $imagick->separateImageChannel($channel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

transformImageColorspace($colorSpace, $channel) ;
echo "Ok";
?>
--EXPECTF--
Ok