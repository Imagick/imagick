--TEST--
Test Imagick::getConfigureOptions
--SKIPIF--
<?php

require_once(dirname(__FILE__) . '/skipif.inc');

checkClassMethods('Imagick', array('getconfigureoptions'));


?>
--FILE--
<?php

$allOptions = Imagick::getConfigureOptions();

if (!is_array($allOptions)) {
	echo "Failed to return array".PHP_EOL;
	var_dump($options);
}
else if (count($allOptions) == 0) {
	echo "allOptions is empty".PHP_EOL;
}


$optionsStartingWithC = Imagick::getConfigureOptions("C*");

if (!is_array($optionsStartingWithC)) {
	echo "Failed to return array".PHP_EOL;
	var_dump($optionsStartingWithC);
}
else if (count($optionsStartingWithC) == 0) {
	echo "optionsStartingWithC is empty".PHP_EOL;
}

if (!(count($optionsStartingWithC) < count($allOptions))) {
	echo "";
	var_dump($optionsStartingWithC);
	var_dump($allOptions);
}

foreach ($optionsStartingWithC as $key => $value) {
	$key = strtolower($key);

	if (strpos($key, "c") !== 0) {
		echo "key $key does not start with c".PHP_EOL;
	}
}

echo "Ok";

?>
--EXPECTF--
Ok