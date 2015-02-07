--TEST--
Test Imagick module hasn't broken phpinfo
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

ob_start();
phpinfo();
$contents = ob_get_contents();
ob_end_clean();

if (strpos($contents, 'imagick module => enabled') !== false) {
	echo "Ok";
}
else {
	echo "Imagick was not reported as enabled?";
	var_dump($contents);
}

?>
--EXPECTF--
Ok