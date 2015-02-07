<?php


function setImageTicksPerSecond() {

    $imagick = new \Imagick(realpath("images/coolGif.gif"));
    $totalFrames = $imagick->getNumberImages();
    $frameCount = 0;

    foreach ($imagick as $frame) {
        /** @var $frame \Imagick */

        if ($frameCount < ($totalFrames / 2)) {
            //Modify the frame to be displayed for twice as long as it currently is.
            $frame->setImageTicksPerSecond(50);
        }
        else {
            //Modify the frame to be displayed for half as long as it currently is.
            $frame->setImageTicksPerSecond(200);
        }
        $frameCount++;
    }

    $imagick2 = $imagick->deconstructImages();
    $bytes = $imagick2->getImagesBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setImageTicksPerSecond() ;
echo "Ok";
?>
