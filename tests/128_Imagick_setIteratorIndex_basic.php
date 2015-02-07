<?php

$firstLayer = 3;

    function setIteratorIndex($firstLayer) {
        $imagick = new \Imagick(realpath("images/LayerTest.psd"));
        $output = new \Imagick();
        $imagick->setIteratorIndex($firstLayer);

        do {
            $output->addImage($imagick->getimage());
        } while($imagick->nextImage());

        $merged = $output->mergeImageLayers(\Imagick::LAYERMETHOD_MERGE);

        $merged->setImageFormat('png');
    $bytes = $merged->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
    }

setIteratorIndex($firstLayer) ;
echo "Ok";
?>
