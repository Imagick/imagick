--TEST--
Test Imagick, setOption
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$imageOption = 0;
$format = 'png64';

function renderPNG($format) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->setImageFormat('png');
    $imagick->setOption('png:format', $format);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

renderPNG($format) ;
echo "Ok";
?>
--EXPECTF--
Ok