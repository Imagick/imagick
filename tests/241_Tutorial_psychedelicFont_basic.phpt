--TEST--
Test Tutorial, psychedelicFont
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function psychedelicFont() {
    $draw = new \ImagickDraw();
    $name = 'Danack';

    $draw->setStrokeOpacity(1);
    $draw->setFillColor('black');

    $draw->setfontsize(150);

    for ($strokeWidth = 25; $strokeWidth > 0; $strokeWidth--) {
        $hue = intval(170 + $strokeWidth * 360 / 25);
        $draw->setStrokeColor("hsl($hue, 255, 128)");
        $draw->setStrokeWidth($strokeWidth * 3);
        $draw->annotation(60, 165, $name);
    }

    //Create an image object which the draw commands can be rendered into
    $imagick = new \Imagick();
    $imagick->newImage(650, 230, "#eee");
    $imagick->setImageFormat("png");

    //Render the draw commands in the ImagickDraw object
    //into the image.
    $imagick->drawImage($draw);

    //Send the image to the browser
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

psychedelicFont() ;
echo "Ok";
?>
--EXPECTF--
Ok