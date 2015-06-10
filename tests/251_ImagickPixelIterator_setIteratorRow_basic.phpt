--TEST--
Test ImagickPixelIterator, setIteratorRow
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function setIteratorRow() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imageIterator = $imagick->getPixelRegionIterator(200, 100, 200, 200);

    for ($x = 0; $x < 20; $x++) {        
        $imageIterator->setIteratorRow($x * 5);
        $pixels = $imageIterator->getCurrentIteratorRow();
        /* Loop through the pixels in the row (columns) */
        foreach ($pixels as $pixel) {
            /** @var $pixel \ImagickPixel */
            /* Paint every second pixel black*/
            $pixel->setColor("rgba(0, 0, 0, 0)"); 
        }

        /* Sync the iterator, this is important to do on each iteration */
        $imageIterator->syncIterator();
    }

    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setIteratorRow() ;
echo "Ok";
?>
--EXPECTF--
Ok