--TEST--
Test clone length
--XFAIL-- 
Image length should not be the same after clone but it is
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$im = new Imagick ('magick:rose');
$im->setImageFormat ('png');
var_dump ($im->getImageLength ());

$cloned = clone $im;
$cloned->setImageFormat ('png');

var_dump ($cloned->getImageLength ());

?>
--EXPECT--
int(9673)
int(0)
