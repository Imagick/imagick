--TEST--
Test ImagickPixelIterator, clear
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function clear() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

    $imageIterator = $imagick->getPixelRegionIterator(100, 100, 250, 200);

    /* Loop trough pixel rows */
    foreach ($imageIterator as $pixels) { 
        /** @var $pixel \ImagickPixel */
        /* Loop through the pixels in the row (columns) */
        foreach ($pixels as $column => $pixel) { 
            if ($column % 2) {
                /* Paint every second pixel black*/
                $pixel->setColor("rgba(0, 0, 0, 0)"); 
            }
        }
        /* Sync the iterator, this is important to do on each iteration */
        $imageIterator->syncIterator();
    }

    $imageIterator->clear();

    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

clear() ;
echo "Ok";
?>
--EXPECTF--
Ok