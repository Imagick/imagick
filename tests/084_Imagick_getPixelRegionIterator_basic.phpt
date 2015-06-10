--TEST--
Test Imagick, getPixelRegionIterator
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function getPixelRegionIterator() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imageIterator = $imagick->getPixelRegionIterator(100, 100, 200, 200);

    /** @noinspection PhpUnusedLocalVariableInspection */
    foreach ($imageIterator as $row => $pixels) { /* Loop trough pixel rows */
        foreach ($pixels as $column => $pixel) { /* Loop through the pixels in the row (columns) */
            /** @var $pixel \ImagickPixel */
            if ($column % 2) {
                $pixel->setColor("rgba(0, 0, 0, 0)"); /* Paint every second pixel black*/
            }
        }
        $imageIterator->syncIterator(); /* Sync the iterator, this is important to do on each iteration */
    }

    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

getPixelRegionIterator() ;
echo "Ok";
?>
--EXPECTF--
Ok