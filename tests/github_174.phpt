--TEST--
Imagick::resizeImage prevent 0 width/height images
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php
$i = new Imagick;
$i->newPseudoImage(1, 1000, "xc:red");
$i->resizeImage(0, 250, 0, 1);
echo $i->getImageWidth(); // should be 1
?>
--EXPECTF--
1
