--TEST--
Test Imagick::getfeatures
--SKIPIF--
<?php

require_once(dirname(__FILE__) . '/skipif.inc');

checkClassMethods('Imagick', array('getfeatures'));


?>
--FILE--
<?php

$features = Imagick::getFeatures();

if (!is_string($features)) {
	echo "Features failed to return a string";
}

echo "Ok";

?>
--EXPECTF--
Ok