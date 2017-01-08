--TEST--
ImagickPixel iterator
--SKIPIF--
<?php 
require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--XFAIL--
This needs a significant amount of work to fix. The issue is the iterator object is going out of scope and being freed, but the row object still has a reference to that pixel iterator,  internally in ImageMagick. We need to stop the pixeliterator being freed while the row is still active.
--FILE--
<?php

//$im = new Imagick();
//$im->newImage(1, 1, 'black');
//
//// This works fine
//$it = $im->getPixelIterator();
//$row = $it->getCurrentIteratorRow();
//$rgb = $row[0]->getColor();
//
//$expected = array (
//  "r" => 0,
//  "g" => 0,
//  "b" => 0,
//  "a" => 1,
//);
//
//if ($rgb !== $expected) {
//	echo "values are incorrect:\n";
//	var_dump($rgb);
//}
//
//// This crashes with SIGABRT
//$row = $im->getPixelIterator()->getCurrentIteratorRow();
//$rgb = $row[0]->getColor();
//
//if ($rgb !== $expected) {
//    echo "values are incorrect:\n";
//    var_dump($rgb);
//}

echo "Not ok";

?>
--EXPECTF--
Not ok