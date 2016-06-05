--TEST--
Test identifyImageType
--SKIPIF--
<?php 

require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('identifyImageType'));

?>
--FILE--
<?php

$im = new Imagick();
$im->newPseudoImage(100, 100, "magick:logo");

$type = $im->identifyImageType();

if ($type !== Imagick::IMGTYPE_PALETTE) {
	echo "Unexpected type value. Expecting: ".Imagick::IMGTYPE_PALETTE.", but got $type. \n";
}
echo "Ok";

?>
--EXPECTF--
Ok