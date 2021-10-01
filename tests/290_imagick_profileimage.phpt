--TEST--
Imagick::profileImage test
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$imagick = new Imagick();
$imagick->newPseudoImage(640, 480, "magick:logo");
$imagick->profileImage('*', null);

echo "Ok";
?>
--EXPECTF--
Ok