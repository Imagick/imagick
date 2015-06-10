--TEST--
Test Imagick, recolorImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('recolorImage'));
?>
--FILE--
<?php


function recolorImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $remapColor = array(
        1, 0, 0,
        0, 0, 1,
        0, 1, 0,
    );

    @$imagick->recolorImage($remapColor);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

recolorImage() ;
echo "Ok";
?>
--EXPECTF--
Ok