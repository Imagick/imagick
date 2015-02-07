<?php


function imagickComposite() {
    //Load the images
    $left = new \Imagick(realpath('images/im/holocaust_tn.gif'));
    $right = new \Imagick(realpath('images/im/spiral_stairs_tn.gif'));
    $gradient = new \Imagick(realpath('images/im/overlap_mask.png'));

    //The right bit will be offset by a certain amount - avoid recalculating.
    $offsetX = $gradient->getImageWidth() - $right->getImageWidth();


    //Fade out the left part - need to negate the mask to
    //make math correct
    $gradient2 = clone $gradient;
    $gradient2->negateimage(false);
    $left->compositeimage($gradient2, \Imagick::COMPOSITE_COPYOPACITY, 0, 0);

    //Fade out the right part
    $right->compositeimage($gradient, \Imagick::COMPOSITE_COPYOPACITY, -$offsetX, 0);

    //Create a new canvas to render everything in to.
    $canvas = new \Imagick();
    $canvas->newImage($gradient->getImageWidth(), $gradient->getImageHeight(), new \ImagickPixel('black'));

    //Blend left half into final image
    $canvas->compositeimage($left, \Imagick::COMPOSITE_BLEND, 0, 0);

    //Blend Right half into final image
    $canvas->compositeimage($right, \Imagick::COMPOSITE_BLEND, $offsetX, 0);

    //Output the final image
    $canvas->setImageFormat('png');

    $bytes = $canvas->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

imagickComposite() ;
echo "Ok";
?>
