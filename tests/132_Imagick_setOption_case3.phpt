--TEST--
Test Imagick, setOption
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$imageOption = 0;

function renderCustomBitDepthPNG() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->setImageFormat('png');
    $imagick->setOption('png:bit-depth', '16');
    $imagick->setOption('png:color-type', 6);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

renderCustomBitDepthPNG() ;
echo "Ok";
?>
--EXPECTF--
Ok