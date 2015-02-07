--TEST--
Test Imagick, quantizeImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$numberColors = 64;
$colorSpace = 1;
$treeDepth = 0;
$dither = 1;

function quantizeImage($numberColors, $colorSpace, $treeDepth, $dither) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->quantizeImage($numberColors, $colorSpace, $treeDepth, $dither, false);
    $imagick->setImageFormat('png');
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

quantizeImage($numberColors, $colorSpace, $treeDepth, $dither) ;
echo "Ok";
?>
--EXPECTF--
Ok