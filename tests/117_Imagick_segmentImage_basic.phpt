--TEST--
Test Imagick, segmentImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$clusterThreshold = 5;
$smoothThreshold = 5;
$colorSpace = 1;

function segmentImage($colorSpace, $clusterThreshold, $smoothThreshold) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->segmentImage($colorSpace, $clusterThreshold, $smoothThreshold);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

segmentImage($colorSpace, $clusterThreshold, $smoothThreshold) ;
echo "Ok";
?>
--EXPECTF--
Ok