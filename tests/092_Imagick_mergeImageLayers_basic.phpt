--TEST--
Test Imagick, mergeImageLayers
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$layerMethodType = 13;

function mergeImageLayers($layerMethodType) {

    $imagick = new \Imagick();
    $nextImage = null;
    $imageNames = array(
        "magick:NETSCAPE",
        "magick:logo",
        "magick:GRANITE",
    );

    foreach ($imageNames as $imageName) {        
        $nextImage = new \Imagick();
        $nextImage->newPseudoImage(640, 480, $imageName);
        $imagick->addImage($nextImage);
    }

    $imagick->resetIterator();
    
    $imagick->setImageFormat('png');

    $result = $imagick->mergeImageLayers($layerMethodType);
    $bytes = $result->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

mergeImageLayers($layerMethodType);
echo "Ok";
?>
--EXPECTF--
Ok