--TEST--
Test Imagick, radialBlurImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php


function radialBlurImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->radialBlurImage(3);
    $imagick->radialBlurImage(5);
    $imagick->radialBlurImage(7);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

radialBlurImage() ;
echo "Ok";
?>
--EXPECTF--
Ok