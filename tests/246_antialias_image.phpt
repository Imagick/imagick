--TEST--
Test pseudo formats
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$im = new Imagick();
$im->newPseudoImage(0, 0, "magick:logo");
//On by default
var_export($im->getAntiAlias());
echo PHP_EOL;

//Turn off
$im->setAntiAlias(false);
var_export($im->getAntiAlias());
echo PHP_EOL;

//Turn back on
$im->setAntiAlias(true);
var_export($im->getAntiAlias());
echo PHP_EOL;

?>
--EXPECTF--
true
false
true