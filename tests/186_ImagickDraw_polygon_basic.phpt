--TEST--
Test ImagickDraw, polygon
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function polygon($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeOpacity(1);
    $draw->setStrokeColor($strokeColor);
    $draw->setStrokeWidth(4);

    $draw->setFillColor($fillColor);

    $points = array(
        array('x' => 40 * 5, 'y' => 10 * 5),
        array('x' => 20 * 5, 'y' => 20 * 5),
        array('x' => 70 * 5, 'y' => 50 * 5),
        array('x' => 60 * 5, 'y' => 15 * 5),
    );

    $draw->polygon($points);

    $image = new \Imagick();
    $image->newImage(500, 300, $backgroundColor);
    $image->setImageFormat("png");
    $image->drawImage($draw);

    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

polygon($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok