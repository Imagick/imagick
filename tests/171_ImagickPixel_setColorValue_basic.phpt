--TEST--
Test ImagickPixel, setColorValue
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php


function setColorValue() {
    $image = new \Imagick();
    $draw = new \ImagickDraw();

    $color = new \ImagickPixel('blue');
    $color->setcolorValue(\Imagick::COLOR_RED, 128);

    $draw->setstrokewidth(1.0);
    $draw->setStrokeColor($color);
    $draw->setFillColor($color);
    $draw->rectangle(200, 200, 300, 300);

    $image->newImage(500, 500, "SteelBlue2");
    $image->setImageFormat("png");
    $image->drawImage($draw);

    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setColorValue() ;
echo "Ok";
?>
--EXPECTF--
Ok