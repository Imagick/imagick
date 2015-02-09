--TEST--
Test Tutorial, imagickCompositeGen
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$contrast = 10;
$blendMidpoint = 0.5;

function generateBlendImage($height, $overlap, $contrast = 10, $midpoint = 0.5) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage($height, $overlap, 'gradient:black-white');
    $quantum = $imagick->getQuantum();
    $imagick->sigmoidalContrastImage(true, $contrast, $midpoint * $quantum);

    return $imagick;
}


function mergeImages($outputSize, $overlap, $contrast = 10, $blendMidpoint = 0.5, $horizontal = true) {

    $images = array();
    $newImageWidth = 0;
    $newImageHeight = 0;

    if ($horizontal == true) {
        $resizeWidth = 0;
        $resizeHeight = $outputSize;
    }
    else {
        $resizeWidth = $outputSize;
        $resizeHeight = 0;
    }

    $blendWidth = 0;

    $srcImages = array(1, 2, 3);

    foreach ($srcImages as $srcImage) {
        $nextImage = new \Imagick();
        $nextImage->newPseudoImage(640, 480, "magick:logo");
        
        
        $nextImage->resizeImage($resizeWidth, $resizeHeight, \Imagick::FILTER_LANCZOS, 0.5);

        if ($horizontal == true) {
            $newImageWidth += $nextImage->getImageWidth();
            $blendWidth = $nextImage->getImageHeight();
        }
        else {
            //$newImageWidth = $nextImage->getImageWidth();
            $blendWidth = $nextImage->getImageWidth();
            $newImageHeight += $nextImage->getImageHeight();
        }

        $images[] = $nextImage;
    }

    if ($horizontal == true) {
        $newImageWidth -= $overlap * (count($srcImages) - 1);
        $newImageHeight = $outputSize;
    }
    else {
        $newImageWidth = $outputSize;
        $newImageHeight -= $overlap * (count($srcImages) - 1);
    }

    if ($blendWidth == 0) {
        throw new \Exception("Failed to read source images");
    }

    $fadeLeftSide = generateBlendImage($blendWidth, $overlap, $contrast, $blendMidpoint);

    if ($horizontal == true) {
        //We are placing the images horizontally.
        $fadeLeftSide->rotateImage('black', -90);
    }

    //Fade out the left part - need to negate the mask to
    //make math correct
    $fadeRightSide = clone $fadeLeftSide;
    $fadeRightSide->negateimage(false);

    //Create a new canvas to render everything in to.
    $canvas = new \Imagick();
    $canvas->newImage($newImageWidth, $newImageHeight, new \ImagickPixel('black'));

    $count = 0;

    $imagePositionX = 0;
    $imagePositionY = 0;

    /** @var $image \Imagick */
    foreach ($images as $image) {
        $finalBlending = new \Imagick();
        $finalBlending->newImage($image->getImageWidth(), $image->getImageHeight(), 'white');

        if ($count != 0) {
            $finalBlending->compositeImage($fadeLeftSide, \Imagick::COMPOSITE_ATOP, 0, 0);
        }

        $offsetX = 0;
        $offsetY = 0;

        if ($horizontal == true) {
            $offsetX = $image->getImageWidth() - $overlap;
        }
        else {
            $offsetY = $image->getImageHeight() - $overlap;
        }

        if ($count != count($images) - 1) {
            $finalBlending->compositeImage($fadeRightSide, \Imagick::COMPOSITE_ATOP, $offsetX, $offsetY);
        }

        $image->compositeImage($finalBlending, \Imagick::COMPOSITE_COPYOPACITY, 0, 0);
        $canvas->compositeimage($image, \Imagick::COMPOSITE_BLEND, $imagePositionX, $imagePositionY);

        if ($horizontal == true) {
            $imagePositionX = $imagePositionX + $image->getImageWidth() - $overlap;
        }
        else {
            $imagePositionY = $imagePositionY + $image->getImageHeight() - $overlap;
        }
        $count++;
    }

    return $canvas;
}

function imagickCompositeGen($contrast = 10, $blendMidpoint = 0.5) {

    $size = 160;

    //Load the images 
    $output = mergeImages(
        $size,
        0.2 * $size, //overlap
        $contrast,
        $blendMidpoint,
        true);

    //$output = generateBlendImage(200, 200, 5, 0.5);
    $output->setImageFormat('png');

    $bytes = $output->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

imagickCompositeGen($contrast = 10, $blendMidpoint = 0.5) ;
echo "Ok";
?>
--EXPECTF--
Ok