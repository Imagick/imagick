--TEST--
Test ImagickPixelIterator, getNextIteratorRow
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function getNextIteratorRow() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imageIterator = $imagick->getPixelIterator();

    $count = 0;
    while ($pixels = $imageIterator->getNextIteratorRow()) {
        if (($count % 3) == 0) {
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

        $count += 1;
    }

    $bytes = $imagick;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

getNextIteratorRow() ;
echo "Ok";
?>
--EXPECTF--
Ok