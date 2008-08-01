--TEST--
Imagick::readImage test
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php
print "--- Catch exception with try/catch\n";
$imagick = new Imagick();
try {
	$imagick->readImage('foo.jpg');
} catch (ImagickException $e) {
	echo $e->getMessage() , "\n";
}

?>
--EXPECTF--
--- Catch exception with try/catch
unable to open image `%s': No such file or directory