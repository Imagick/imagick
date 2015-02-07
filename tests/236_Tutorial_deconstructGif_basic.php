<?php

$deconstruct = ;

function makeSimpleGif($deconstruct) {
    $aniGif = new \Imagick();
    $aniGif->setFormat("gif");

    $circleRadius = 20;
    $imageFrames = 40;
    $imageSize = 200;

    $background = new \Imagick();
    $background->newpseudoimage($imageSize, $imageSize, "plasma:tomato-steelblue");

    $blackWhite = new \Imagick();
    $blackWhite->newpseudoimage($imageSize, $imageSize, "gradient:black-white");

    $backgroundPalette = clone $background;
    $backgroundPalette->quantizeImage(240, \Imagick::COLORSPACE_RGB, 8, false, false);

    $blackWhitePalette = clone $blackWhite;
    $blackWhitePalette->quantizeImage(16, \Imagick::COLORSPACE_RGB, 8, false, false);

    $backgroundPalette->addimage($blackWhitePalette);

    for($count=0 ; $count<$imageFrames ; $count++){
        $drawing = new \ImagickDraw();
        $drawing->setFillColor('white');
        $drawing->setStrokeColor('rgba(64, 64, 64, 0.8)');
        $strokeWidth = 4;
        $drawing->setStrokeWidth($strokeWidth);
        
        $distanceToMove = $imageSize + (($circleRadius + $strokeWidth) * 2);
        $offset = ($distanceToMove * $count / ($imageFrames -1)) - ($circleRadius + $strokeWidth);
        $drawing->translate($offset, ($imageSize / 2) + ($imageSize / 3 * cos(20 * $count / $imageFrames)));
        $drawing->circle(0, 0, $circleRadius, 0);

        $frame = clone $background;
        $frame->drawimage($drawing);
        $frame->clutimage($backgroundPalette);
        $frame->setImageDelay(10);
        $aniGif->addImage($frame);
    }

    if ($deconstruct == true) {
        $aniGif = $aniGif->deconstructImages();
    }

    $bytes = $aniGif->getImagesBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

makeSimpleGif($deconstruct) ;
echo "Ok";
?>
