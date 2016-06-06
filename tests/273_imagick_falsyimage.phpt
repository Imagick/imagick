--TEST--
Imagick::__construct false
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

try {
	$imagick = new Imagick(false);
}
catch (\ImagickException $ie) {
	echo $ie->getMessage()."\n";
}


?>
--EXPECTF--
Constructor shouldn't be called with a boolean as the filename