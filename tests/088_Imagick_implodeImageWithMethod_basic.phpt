--TEST--
Test Imagick, implodeImageWithMethod
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function implodeImage($method) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->implodeImageWithMethod(1.15, $method);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
//    $imagick->writeImage(__DIR__ . "/implodeImageWithMethod_$method.png");
}

$methods = [
    Imagick::INTERPOLATE_SPLINE,
    Imagick::INTERPOLATE_AVERAGE_16,
    Imagick::INTERPOLATE_BACKGROUND_COLOR
];

foreach ($methods as $method) {
    implodeImage($method);
}
echo "Ok";
?>
--EXPECTF--
Ok