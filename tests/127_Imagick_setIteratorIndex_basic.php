<?php

$firstLayer = 2;

    function setIteratorIndex($firstLayer) {
        $imagick = new \Imagick();
        $imagick->newPseudoImage(640, 480, "magick:logo");',
        
        $imagick2 = new \Imagick();
        $imagick2->newPseudoImage(640, 480, "magick:ROSE");
        
        $imagick3 = new \Imagick();
        $imagick3->newPseudoImage(640, 480, "magick:ROSE");
        
        $imagick->addImage($imagick2);
        $imagick->addImage($imagick3);

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
