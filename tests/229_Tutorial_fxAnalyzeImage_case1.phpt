--TEST--
Test Tutorial, fxAnalyzeImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

ini_set('memory_limit','512M');

// Analyzes a one pixel wide image to make it easy to see what the
// gradient is doing
function fxAnalyzeImage(\Imagick $imagick) {
    
    $graphWidth = $imagick->getImageWidth();
    $sampleHeight = 20;
    $graphHeight = 128;
    $border = 2;

    $imageIterator = new \ImagickPixelIterator($imagick);

    $reds = array();

    foreach ($imageIterator as $pixels) { /* Loop trough pixel rows */
        foreach ($pixels as $pixel) { /* Loop through the pixels in the row (columns) */
            /** @var $pixel \ImagickPixel */
            $color = $pixel->getColor();
            $reds[] = $color['r'];
        }
        $imageIterator->syncIterator(); /* Sync the iterator, this is important to do on each iteration */
    }

    $draw = new \ImagickDraw();

    $strokeColor = new \ImagickPixel('red');
    $fillColor = new \ImagickPixel('none');
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(1);
    $draw->setFontSize(72);
    $draw->setStrokeAntiAlias(true);

    $x = 0;
    $points = array();
    
    foreach ($reds as $red) {
        $pos = $graphHeight - ($red * $graphHeight / 256);
        $points[] = array('x' => $x, 'y' => $pos);
        $x += 1;
    }

    $draw->polyline($points);

    $plot = new \Imagick();
    $plot->newImage($graphWidth, $graphHeight, 'white');
    $plot->drawImage($draw);

    $outputImage = new \Imagick();
    $outputImage->newImage($graphWidth, $graphHeight + $sampleHeight, 'white');
    $outputImage->compositeimage($plot, \Imagick::COMPOSITE_ATOP, 0, 0);

    $imagick->resizeimage($imagick->getImageWidth(), $sampleHeight, \Imagick::FILTER_LANCZOS, 1);

    $outputImage->compositeimage($imagick, \Imagick::COMPOSITE_ATOP, 0, $graphHeight);
    $outputImage->borderimage('black', $border, $border);

    $outputImage->setImageFormat("png");
    $bytes = $outputImage;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}


$arguments = array(5, 1, 0.5);

$imagick = new \Imagick();
$imagick->newPseudoImage(500, 500, 'gradient:black-white');
$imagick->functionImage(\Imagick::FUNCTION_POLYNOMIAL, $arguments);
$imagick->setimageformat('png');

fxAnalyzeImage($imagick);

echo "Ok";
?>
--EXPECTF--
Ok