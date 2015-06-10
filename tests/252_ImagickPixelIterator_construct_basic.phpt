--TEST--
Test ImagickPixelIterator, construct
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function construct() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imageIterator = new \ImagickPixelIterator($imagick);

    /* Loop through pixel rows */
    foreach ($imageIterator as $pixels) { 
        /* Loop through the pixels in the row (columns) */
        foreach ($pixels as $column => $pixel) { 
            /** @var $pixel \ImagickPixel */
            if ($column % 2) {
                /* Paint every second pixel black*/
                $pixel->setColor("rgba(0, 0, 0, 0)");
            }
        }
        /* Sync the iterator, this is important to do on each iteration */
        $imageIterator->syncIterator();
    }

    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

construct() ;
echo "Ok";
?>
--EXPECTF--
Ok