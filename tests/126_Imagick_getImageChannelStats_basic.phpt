--TEST--
Test Imagick, setImageOrientation
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php


$imagick = new \Imagick();
$imagick->newPseudoImage(640, 480, "magick:logo");
$identifyInfo = $imagick->getImageChannelStatistics();
var_dump($identifyInfo);

echo "Ok";
?>
--EXPECTF--
Ok