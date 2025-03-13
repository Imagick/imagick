--TEST--
Test Imagick::optimizeimagelayers and Imagick::optimizeimagetransparency
--SKIPIF--
<?php

$imageMagickRequiredVersion = 0x686;

require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('optimizeimagelayers'));
checkClassMethods('Imagick', array('optimizeimagetransparency'));

?>
--FILE--
<?php

function makeSimpleGif() {
    $aniGif = new \Imagick();
    $aniGif->setFormat("gif");

    $circleRadius = 20;
    $imageFrames = 6;
    $imageSize = 200;

    $background = new \Imagick();
    $background->newpseudoimage($imageSize, $imageSize, "canvas:gray");

    $blackWhite = new \Imagick();
    $blackWhite->newpseudoimage($imageSize, $imageSize, "gradient:black-white");

    $backgroundPalette = clone $background;
    $backgroundPalette->quantizeImage(240, \Imagick::COLORSPACE_RGB, 8, false, false);

    $blackWhitePalette = clone $blackWhite;
    $blackWhitePalette->quantizeImage(16, \Imagick::COLORSPACE_RGB, 8, false, false);

    $backgroundPalette->addimage($blackWhitePalette);

    for($count=0 ; $count<$imageFrames ; $count++) {
        echo "Frame: ".$count."\n";
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

    $aniGif = $aniGif->deconstructImages();

    $bytes = $aniGif->getImagesBlob();
    if (strlen($bytes) <= 0) {
        echo "Failed to generate image.";
    }

    return $aniGif;
}

function optimizeGif($im) {
    $im = $im->coalesceImages();
    $im->optimizeImageLayers();
    $im->optimizeimagetransparency();
}

$simpleGif = makeSimpleGif();
optimizeGif($simpleGif);

echo "Ok"
?>
--EXPECT--
Frame: 0
Frame: 1
Frame: 2
Frame: 3
Frame: 4
Frame: 5
Ok
