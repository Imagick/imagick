--TEST--
Test Imagick, resizeImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$filterType = 22;
$width = 200;
$height = 200;
$blur = 1;
$bestFitSettings = array(0, 1);
$cropZoom = 1;

function resizeImage($width, $height, $filterType, $blur, $bestFit, $cropZoom) {
    //The blur factor where &gt; 1 is blurry, &lt; 1 is sharp.
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

    $imagick->resizeImage($width, $height, $filterType, $blur, $bestFit);

    $cropWidth = $imagick->getImageWidth();
    $cropHeight = $imagick->getImageHeight();

    if ($cropZoom) {
        $newWidth = $cropWidth / 2;
        $newHeight = $cropHeight / 2;

        $imagick->cropimage(
            $newWidth,
            $newHeight,
            ($cropWidth - $newWidth) / 2,
            ($cropHeight - $newHeight) / 2
        );

        $imagick->scaleimage(
            $imagick->getImageWidth() * 4,
            $imagick->getImageHeight() * 4
        );
    }


    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

foreach ($bestFitSettings as $bestFit) {
	resizeImage($width, $height, $filterType, $blur, $bestFit, $cropZoom);
}
echo "Ok";
?>
--EXPECTF--
Ok