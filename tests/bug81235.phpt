--TEST--
Bug #81235 (Imagick::newPseudoImage gives open_basedir restriction error)
--SKIPIF--
<?php require_once(__DIR__ . '/skipif.inc'); ?>
--FILE--
<?php
ini_set('open_basedir', __DIR__);
$imagick = new \Imagick();
$imagick->newPseudoImage(10, 10, "canvas:white");
?>
--EXPECT--
