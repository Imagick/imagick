--TEST--
Test ImagickDraw, setStrokeMiterLimit
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function setStrokeMiterLimit($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeColor($strokeColor);
    $draw->setStrokeOpacity(0.6);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(10);

    $yOffset = 100;

    $draw->setStrokeLineJoin(\Imagick::LINEJOIN_MITER);

    for ($y = 0; $y < 3; $y++) {

        $draw->setStrokeMiterLimit(40 * $y);

        $points = array(
            array('x' => 22 * 3, 'y' => 15 * 4 + $y * $yOffset),
            array('x' => 20 * 3, 'y' => 20 * 4 + $y * $yOffset),
            array('x' => 70 * 5, 'y' => 45 * 4 + $y * $yOffset),
        );

        $draw->polygon($points);
    }

    $image = new \Imagick();
    $image->newImage(500, 500, $backgroundColor);
    $image->setImageFormat("png");
    $image->drawImage($draw);

    $image->setImageType(\Imagick::IMGTYPE_PALETTE);
    //TODO - this should either be everywhere or nowhere
    $image->setImageCompressionQuality(100);
    $image->stripImage();

    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setStrokeMiterLimit($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok