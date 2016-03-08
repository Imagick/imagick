--TEST--
Test autoGammaImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('autoGammaImage'));
?>
--FILE--
<?php

$im = new Imagick("magick:logo");
$im->autoGammaImage();


$im = new Imagick("magick:logo");
$im->autoGammaImage(\Imagick::CHANNEL_RED);

echo "Ok";

?>
--EXPECT--
Ok