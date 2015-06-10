--TEST--
Test ImagickPixelIterator, resetIterator
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function resetIterator() {

    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

    $imageIterator = $imagick->getPixelIterator();

    /* Loop trough pixel rows */
    foreach ($imageIterator as $pixels) {
        /* Loop through the pixels in the row (columns) */
        foreach ($pixels as $column => $pixel) {
            /** @var $pixel \ImagickPixel */
            if ($column % 2) {

                /* Make every second pixel 25% red*/
                $pixel->setColorValue(\Imagick::COLOR_RED, 64); 
            }
        }
        /* Sync the iterator, this is important to do on each iteration */
        $imageIterator->syncIterator();
    }

    $imageIterator->resetiterator();

    /* Loop trough pixel rows */
    foreach ($imageIterator as $pixels) {
        /* Loop through the pixels in the row (columns) */
        foreach ($pixels as $column => $pixel) {
            /** @var $pixel \ImagickPixel */
            if ($column % 3) {
                $pixel->setColorValue(\Imagick::COLOR_BLUE, 64); /* Make every second pixel a little blue*/
                //$pixel->setColor("rgba(0, 0, 128, 0)"); /* Paint every second pixel black*/
            }
        }
        $imageIterator->syncIterator(); /* Sync the iterator, this is important to do on each iteration */
    }

    $imageIterator->clear();

    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

resetIterator() ;
echo "Ok";
?>
--EXPECTF--
Ok