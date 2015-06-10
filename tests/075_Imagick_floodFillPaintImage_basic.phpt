--TEST--
Test Imagick, floodFillPaintImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$fillColor = 'rgb(0, 0, 0)';
$fuzz = 0.2;
$targetColor = 'rgb(245, 124, 24)';
$x = 260;
$y = 150;
$inverse = 0;
$channel = Imagick::CHANNEL_DEFAULT;

function floodFillPaintImage($fillColor, $fuzz, $targetColor, $x, $y, $inverse, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    
    $imagick->floodFillPaintImage(
        $fillColor,
        $fuzz * \Imagick::getQuantum(),
        $targetColor,
        $x, $y,
        $inverse,
        $channel
    );
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

floodFillPaintImage($fillColor, $fuzz, $targetColor, $x, $y, $inverse, $channel) ;
echo "Ok";
?>
--EXPECTF--
Ok