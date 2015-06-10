--TEST--
Test ImagickDraw, setGravity
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

function setGravity($fillColor, $strokeColor, $backgroundColor) {

    $draw = new \ImagickDraw();
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);
    $draw->setStrokeWidth(1);
    $draw->setFontSize(24);

    $gravitySettings = array(\Imagick::GRAVITY_NORTHWEST => 'NorthWest', \Imagick::GRAVITY_NORTH => 'North', \Imagick::GRAVITY_NORTHEAST => 'NorthEast', \Imagick::GRAVITY_WEST => 'West', \Imagick::GRAVITY_CENTER => 'Centre', \Imagick::GRAVITY_SOUTHWEST => 'SouthWest', \Imagick::GRAVITY_SOUTH => 'South', \Imagick::GRAVITY_SOUTHEAST => 'SouthEast', \Imagick::GRAVITY_EAST => 'East');


    foreach ($gravitySettings as $type => $description) {
        $draw->setGravity($type);
        $draw->annotation(50, 50, '"' . $description . '"');
    }

    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");
    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setGravity($fillColor, $strokeColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok