--TEST--
Test Imagick, equalizeImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


//This appears to corrupt the image colors?
function extentImage($startX, $startY, $width, $height) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->equalizeImage();
    $imagick->extentImage(
        $startX, $startY, $width, $height
    );
    
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}
$startX = 50;
$startY = 50;
$width = 150;
$height = 150;

extentImage($startX, $startY, $width, $height) ;
echo "Ok";
?>
--EXPECTF--
Ok