--TEST--
Test Imagick, frameImageWithComposite
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$color = 'rgba(255,0,255,50)';
$width = 5;
$height = 5;
$innerBevel = 3;
$outerBevel = 3;

function frameImage($color, $width, $height, $innerBevel, $outerBevel, $blendOption) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

    $width = $width + $innerBevel + $outerBevel;
    $height = $height + $innerBevel + $outerBevel;

    if ($blendOption === null) {
        $imagick->frameimage(
            $color,
            $width,
            $height,
            $innerBevel,
            $outerBevel
        );
    }
    else {
        $imagick->frameImageWithComposite(
                $color,
                $width,
                $height,
                $innerBevel,
                $outerBevel,
                $blendOption
        );
    }

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) {
        echo "Failed to generate image.";
    }

    return $imagick;
}

$blendOptions = [
    "null" => null,
    "COMPOSITE_NO" => Imagick::COMPOSITE_NO,
    "COMPOSITE_BLEND" => Imagick::COMPOSITE_BLEND,
    "COMPOSITE_COPYRED" => Imagick::COMPOSITE_COPYRED,
    "COMPOSITE_ATOP" => Imagick::COMPOSITE_ATOP,
    "COMPOSITE_OVER" => Imagick::COMPOSITE_OVER,
];

foreach ($blendOptions as $name => $blendOption) {
    $imagick = frameImage($color, $width, $height, $innerBevel, $outerBevel, $blendOption) ;
    $filename = "077_frame_" . $name . ".png";
    // $imagick->writeImage($filename);
}

echo "Ok";
?>
--EXPECTF--
Ok