--TEST--
Test Imagick, transparentPaintImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$color = 'rgb(39, 194, 255)';
$alpha = 0;
$fuzz = 0.1;

function transparentPaintImage($color, $alpha, $fuzz) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

    //Need to be in a format that supports transparency
    $imagick->setimageformat('png');

    $imagick->transparentPaintImage(
        $color, $alpha, $fuzz * \Imagick::getQuantum(), false
    );

    //Not required, but helps tidy up left over pixels
    $imagick->despeckleimage();

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

transparentPaintImage($color, $alpha, $fuzz) ;
echo "Ok";
?>
--EXPECTF--
Ok