--TEST--
Test Imagick, recolorImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php


function recolorImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $remapColor = [ 1, 0, 0,
        0, 0, 1,
        0, 1, 0,];

//$remapColor = [
//    1.438, -0.122, -0.016,  0, 0, -0.03,
//    -0.062,  1.378, -0.016,  0, 0,  0.05,
//    -0.062, -0.122, 1.483,   0, 0, -0.02,
//];

    @$imagick->recolorImage($remapColor);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

recolorImage() ;
echo "Ok";
?>
--EXPECTF--
Ok