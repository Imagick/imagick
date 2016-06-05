--TEST--
Imagick::readImage test
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$reflClass = new ReflectionClass('Imagick');
$constants = $reflClass->getConstants();

$output = '';

foreach ($constants as $key => $value) {
	$output .= "$key: $value \n";
}
echo "Ok";

?>
--EXPECTF--
Ok