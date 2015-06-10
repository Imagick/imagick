--TEST--
Test Imagick, setImageOrientation
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$orientationType = \Imagick::ORIENTATION_LEFTTOP;

//Doesn't appear to do anything
function setImageOrientation($orientationType) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->setImageOrientation($orientationType);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setImageOrientation($orientationType) ;
echo "Ok";
?>
--EXPECTF--
Ok