<?php


function setImageDelay() {
    $imagick = new \Imagick(realpath("images/coolGif.gif"));

    $frameCount = 0;

    foreach ($imagick as $frame) {
        /** @var $frame \Imagick */
        $frame->setImageDelay((($frameCount % 11) * 5));
        $frameCount++;
    }

    $imagick2 = $imagick->deconstructImages();

    $bytes = $imagick2->getImagesBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setImageDelay() ;
echo "Ok";
?>
