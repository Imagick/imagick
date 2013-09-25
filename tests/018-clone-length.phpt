--TEST--
Test static methods
--XFAIL-- 
Image length should be the same after clone but it's not
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$im = new Imagick ('magick:rose');
$im->setImageFormat ('png');
var_dump ($im->getImageLength ());

$cloned = clone $im;
$cloned->setImageFormat ('png');

var_dump ($cloned->getImageLength () == $im->getImageLength ());

?>
--EXPECT--
success