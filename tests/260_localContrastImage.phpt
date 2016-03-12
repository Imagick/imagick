--TEST--
Test localContrastImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc');

$v = Imagick::getVersion();
checkClassMethods('Imagick', array('localContrastImage'));

?>
--FILE--
<?php

$im = new Imagick("magick:logo");
$im->localContrastImage(2, 2);

echo "Ok";

?>
--EXPECT--
Ok