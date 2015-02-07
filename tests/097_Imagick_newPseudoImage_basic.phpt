--TEST--
Test Imagick, newPseudoImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$canvasType = 'gradient:';

function newPseudoImage($canvasType) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(300, 300, $canvasType);
    $imagick->setImageFormat("png");
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

newPseudoImage($canvasType) ;
echo "Ok";
?>
--EXPECTF--
Ok