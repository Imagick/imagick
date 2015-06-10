--TEST--
Test ImagickDraw, setFontSize
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

function setFontSize($fillColor, $strokeColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeOpacity(1);
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(2);

    $sizes = array(24, 36, 48, 60, 72);

    foreach ($sizes as $size) {
        $draw->setFontSize($size);
        $draw->annotation(50, ($size * $size / 16), "Lorem Ipsum!");
    }

    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");
    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setFontSize($fillColor, $strokeColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok