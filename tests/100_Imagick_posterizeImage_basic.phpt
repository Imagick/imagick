--TEST--
Test Imagick, posterizeImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$posterizeType = 2;
$numberLevels = 8;

function posterizeImage($posterizeType, $numberLevels) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->posterizeImage($numberLevels, $posterizeType);
    $imagick->setImageFormat('png');
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

posterizeImage($posterizeType, $numberLevels) ;
echo "Ok";
?>
--EXPECTF--
Ok