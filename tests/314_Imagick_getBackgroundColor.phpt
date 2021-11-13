--TEST--
Test Imagick, getBackgroundColor
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('getBackgroundColor'));
?>
--FILE--
<?php

function getBackgroundColor() {
    $imagick = new \Imagick();
    $background_color = $imagick->getBackgroundColor();

    /** @var $background_color \ImagickPixel */
    echo "Color is: " . $background_color->getColorAsString() . "\n";

    $imagick->setBackgroundColor('red');
    $background_color = $imagick->getBackgroundColor();

    /** @var $background_color \ImagickPixel */
    echo "Color is now: " . $background_color->getColorAsString() . "\n";

}

getBackgroundColor() ;
echo "Ok";
?>
--EXPECTF--
Color is: srgb(255,255,255)
Color is now: srgb(255,0,0)
Ok
