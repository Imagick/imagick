--TEST--
Test Imagick, subImageMatch
--SKIPIF--
<?php
$imageMagickRequiredVersion=675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function subImageMatch() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick2 = clone $imagick;
    $imagick2->cropimage(40, 40, 250, 110);
    $imagick2->vignetteimage(0, 1, 3, 3);

    $similarity = null;
    $bestMatch = null;
    $comparison = $imagick->subImageMatch($imagick2, $bestMatch, $similarity);

    $comparison->setImageFormat('png');
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

subImageMatch() ;
echo "Ok";
?>
--EXPECTF--
Ok