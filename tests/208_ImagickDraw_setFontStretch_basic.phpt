--TEST--
Test ImagickDraw, setFontStretch
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function setFontStretch($fillColor, $strokeColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(2);
    $draw->setFontSize(36);

    $fontStretchTypes = array(\Imagick::STRETCH_ULTRACONDENSED, \Imagick::STRETCH_CONDENSED, \Imagick::STRETCH_SEMICONDENSED, \Imagick::STRETCH_SEMIEXPANDED, \Imagick::STRETCH_EXPANDED, \Imagick::STRETCH_EXTRAEXPANDED, \Imagick::STRETCH_ULTRAEXPANDED, \Imagick::STRETCH_ANY);

    $offset = 0;
    foreach ($fontStretchTypes as $fontStretch) {
        $draw->setFontStretch($fontStretch);
        $draw->annotation(50, 75 + $offset, "Lorem Ipsum!");
        $offset += 50;
    }

    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");
    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setFontStretch($fillColor, $strokeColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok