--TEST--
Check that Imagick is actually loaded
--SKIPIF--

--FILE--
<?php

// The tests are all skipped if imagick is not loaded. This can lead
// to a false green build when Imagick isn't actually loaded. So 
// this script gives an error to allow us to error before running the tests.
if (extension_loaded("imagick")) {
	echo "OK" . PHP_EOL;
}
else {
	echo "imagick module is not loaded." . PHP_EOL;
}

?>
--EXPECTF--
OK