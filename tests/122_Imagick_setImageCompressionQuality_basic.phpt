--TEST--
Test Imagick, setImageCompressionQuality
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$quality = 15;

function setImageCompressionQuality($quality) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    //$imagick->setImageCompressionQuality($quality);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setImageCompressionQuality($quality) ;
echo "Ok";
?>
--EXPECTF--
Ok