--TEST--
Test ImagickDraw, point
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function point($fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setFillColor($fillColor);

    for ($x = 0; $x < 10000; $x++) {
        $draw->point(rand(0, 500), rand(0, 500));
    }

    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");
    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

point($fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok