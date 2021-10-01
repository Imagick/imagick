--TEST--
Test Imagick, borderImageWithComposite
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$width = 50;
$height = 20;
$color = 'rgb(200, 64, 127)';

function borderImage($color, $width, $height, $blendOption) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->borderImage($color, $width, $height);

    if ($blendOption === null) {
        $imagick->borderImage($color, $width, $height);
    }
    else {
        $imagick->borderImageWithComposite($color, $width, $height, $blendOption);
    }

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) {
        echo "Failed to generate image.";
    }

    return $imagick;
}

$blendOptions = [
    "null" => null,
//    "COMPOSITE_NO" => Imagick::COMPOSITE_NO,
//    //"COMPOSITE_ADD" => Imagick::COMPOSITE_ADD,
    "COMPOSITE_ATOP" => Imagick::COMPOSITE_ATOP,
//    "COMPOSITE_BLEND" => Imagick::COMPOSITE_BLEND,
//    "COMPOSITE_BUMPMAP" => Imagick::COMPOSITE_BUMPMAP,
//    "COMPOSITE_CLEAR" => Imagick::COMPOSITE_CLEAR,
//    "COMPOSITE_COLORBURN" => Imagick::COMPOSITE_COLORBURN,
//    "COMPOSITE_COLORDODGE" => Imagick::COMPOSITE_COLORDODGE,
//    "COMPOSITE_COLORIZE" => Imagick::COMPOSITE_COLORIZE,
//    "COMPOSITE_COPYBLACK" => Imagick::COMPOSITE_COPYBLACK,
//    "COMPOSITE_COPYBLUE" => Imagick::COMPOSITE_COPYBLUE,
//    "COMPOSITE_COPY" => Imagick::COMPOSITE_COPY,
//    "COMPOSITE_COPYCYAN" => Imagick::COMPOSITE_COPYCYAN,
//    "COMPOSITE_COPYGREEN" => Imagick::COMPOSITE_COPYGREEN,
//    "COMPOSITE_COPYMAGENTA" => Imagick::COMPOSITE_COPYMAGENTA,
//    "COMPOSITE_COPYALPHA" => Imagick::COMPOSITE_COPYALPHA,
//    "COMPOSITE_COPYOPACITY" => Imagick::COMPOSITE_COPYOPACITY,
//    "COMPOSITE_COPYRED" => Imagick::COMPOSITE_COPYRED,
//    "COMPOSITE_COPYYELLOW" => Imagick::COMPOSITE_COPYYELLOW,
//    "COMPOSITE_DARKEN" => Imagick::COMPOSITE_DARKEN,
//    "COMPOSITE_DSTATOP" => Imagick::COMPOSITE_DSTATOP,
//    "COMPOSITE_DST" => Imagick::COMPOSITE_DST,
//    "COMPOSITE_DSTIN" => Imagick::COMPOSITE_DSTIN,
//    "COMPOSITE_DSTOUT" => Imagick::COMPOSITE_DSTOUT,
//    "COMPOSITE_DSTOVER" => Imagick::COMPOSITE_DSTOVER,
//    "COMPOSITE_DIFFERENCE" => Imagick::COMPOSITE_DIFFERENCE,
// "COMPOSITE_DISPLACE" => Imagick::COMPOSITE_DISPLACE,
//    "COMPOSITE_DISSOLVE" => Imagick::COMPOSITE_DISSOLVE,
//    "COMPOSITE_EXCLUSION" => Imagick::COMPOSITE_EXCLUSION,
//    "COMPOSITE_HARDLIGHT" => Imagick::COMPOSITE_HARDLIGHT,
//    "COMPOSITE_HUE" => Imagick::COMPOSITE_HUE,
//    "COMPOSITE_IN" => Imagick::COMPOSITE_IN,
//    "COMPOSITE_LIGHTEN" => Imagick::COMPOSITE_LIGHTEN,
//    "COMPOSITE_LUMINIZE" => Imagick::COMPOSITE_LUMINIZE,
//    //"COMPOSITE_MINUS" => Imagick::COMPOSITE_MINUS,
//    "COMPOSITE_MODULATE" => Imagick::COMPOSITE_MODULATE,
//    "COMPOSITE_MULTIPLY" => Imagick::COMPOSITE_MULTIPLY,
//    "COMPOSITE_OUT" => Imagick::COMPOSITE_OUT,
    "COMPOSITE_OVER" => Imagick::COMPOSITE_OVER,
//    "COMPOSITE_OVERLAY" => Imagick::COMPOSITE_OVERLAY,
//    "COMPOSITE_PLUS" => Imagick::COMPOSITE_PLUS,
//    "COMPOSITE_REPLACE" => Imagick::COMPOSITE_REPLACE,
//    "COMPOSITE_SATURATE" => Imagick::COMPOSITE_SATURATE,
//    "COMPOSITE_SCREEN" => Imagick::COMPOSITE_SCREEN,
//    "COMPOSITE_SOFTLIGHT" => Imagick::COMPOSITE_SOFTLIGHT,
//    "COMPOSITE_SRCATOP" => Imagick::COMPOSITE_SRCATOP,
//    "COMPOSITE_SRC" => Imagick::COMPOSITE_SRC,
//    "COMPOSITE_SRCIN" => Imagick::COMPOSITE_SRCIN,
//    "COMPOSITE_SRCOUT" => Imagick::COMPOSITE_SRCOUT,
//    "COMPOSITE_SRCOVER" => Imagick::COMPOSITE_SRCOVER,
//    // "COMPOSITE_SUBTRACT" => Imagick::COMPOSITE_SUBTRACT,
//    "COMPOSITE_THRESHOLD" => Imagick::COMPOSITE_THRESHOLD,
//    "COMPOSITE_XOR" => Imagick::COMPOSITE_XOR,
//    "COMPOSITE_CHANGEMASK" => Imagick::COMPOSITE_CHANGEMASK,
//    "COMPOSITE_LINEARLIGHT" => Imagick::COMPOSITE_LINEARLIGHT,
    // "COMPOSITE_DIVIDE" => Imagick::COMPOSITE_DIVIDE,
//     "COMPOSITE_DISTORT" => Imagick::COMPOSITE_DISTORT,
//    //"COMPOSITE_BLUR" => Imagick::COMPOSITE_BLUR,
//    "COMPOSITE_PEGTOPLIGHT" => Imagick::COMPOSITE_PEGTOPLIGHT,
//    "COMPOSITE_VIVIDLIGHT" => Imagick::COMPOSITE_VIVIDLIGHT,
//    "COMPOSITE_PINLIGHT" => Imagick::COMPOSITE_PINLIGHT,
//    "COMPOSITE_LINEARDODGE" => Imagick::COMPOSITE_LINEARDODGE,
//    "COMPOSITE_LINEARBURN" => Imagick::COMPOSITE_LINEARBURN,
//    "COMPOSITE_MATHEMATICS" => Imagick::COMPOSITE_MATHEMATICS,
//    "COMPOSITE_MODULUSADD" => Imagick::COMPOSITE_MODULUSADD,
//    "COMPOSITE_MODULUSSUBTRACT" => Imagick::COMPOSITE_MODULUSSUBTRACT,
//    "COMPOSITE_MINUSDST" => Imagick::COMPOSITE_MINUSDST,
//    "COMPOSITE_DIVIDEDST" => Imagick::COMPOSITE_DIVIDEDST,
//    "COMPOSITE_DIVIDESRC" => Imagick::COMPOSITE_DIVIDESRC,
//    "COMPOSITE_MINUSSRC" => Imagick::COMPOSITE_MINUSSRC,
//    "COMPOSITE_DARKENINTENSITY" => Imagick::COMPOSITE_DARKENINTENSITY,
//    "COMPOSITE_LIGHTENINTENSITY" => Imagick::COMPOSITE_LIGHTENINTENSITY,
//    "COMPOSITE_HARDMIX" => Imagick::COMPOSITE_HARDMIX,
//    "COMPOSITE_STEREO" => Imagick::COMPOSITE_STEREO,
//    "COMPOSITE_FREEZE" => Imagick::COMPOSITE_FREEZE,
//    "COMPOSITE_INTERPOLATE" => Imagick::COMPOSITE_INTERPOLATE,
//    "COMPOSITE_NEGATE" => Imagick::COMPOSITE_NEGATE,
//    "COMPOSITE_REFLECT" => Imagick::COMPOSITE_REFLECT,
//    "COMPOSITE_SOFTBURN" => Imagick::COMPOSITE_SOFTBURN,
//    "COMPOSITE_SOFTDODGE" => Imagick::COMPOSITE_SOFTDODGE,
//    "COMPOSITE_STAMP" => Imagick::COMPOSITE_STAMP,
//    "COMPOSITE_RMSE" => Imagick::COMPOSITE_RMSE,
];

foreach ($blendOptions as $name => $blendOption) {
    // echo "name: $name \n";
    $imagick = borderImage($color, $width, $height, $blendOption);
    $filename = "039_border_" . $name . ".png";
    // $imagick->writeImage($filename);
}

echo "Ok";
?>
--EXPECTF--
Ok