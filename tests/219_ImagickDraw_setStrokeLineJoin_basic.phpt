--TEST--
Test ImagickDraw, setStrokeLineJoin
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function setStrokeLineJoin($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();
    $draw->setStrokeWidth(1);
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);

    $draw->setStrokeWidth(20);

    $offset = 220;

    $lineJoinStyle = array(\Imagick::LINEJOIN_MITER, \Imagick::LINEJOIN_ROUND, \Imagick::LINEJOIN_BEVEL,);

    for ($x = 0; $x < count($lineJoinStyle); $x++) {
        $draw->setStrokeLineJoin($lineJoinStyle[$x]);
        $points = array(
            array('x' => 40 * 5, 'y' => 10 * 5 + $x * $offset),
            array('x' => 20 * 5, 'y' => 20 * 5 + $x * $offset),
            array('x' => 70 * 5, 'y' => 50 * 5 + $x * $offset),
            array('x' => 40 * 5, 'y' => 10 * 5 + $x * $offset),
        );

        $draw->polyline($points);
    }

    $image = new \Imagick();
    $image->newImage(500, 700, $backgroundColor);
    $image->setImageFormat("png");

    $image->drawImage($draw);

    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setStrokeLineJoin($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok