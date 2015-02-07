--TEST--
Test ImagickPixel, setColor
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php


function setColor() {
    $draw = new \ImagickDraw();

    $strokeColor = new \ImagickPixel('green');
    $fillColor = new \ImagickPixel();
    $fillColor->setColor('rgba(100%, 75%, 0%, 1.0)');

    $draw->setstrokewidth(3.0);
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->rectangle(200, 200, 300, 300);

    $image = new \Imagick();
    $image->newImage(500, 500, "SteelBlue2");
    $image->setImageFormat("png");

    $image->drawImage($draw);

    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setColor() ;
echo "Ok";
?>
--EXPECTF--
Ok