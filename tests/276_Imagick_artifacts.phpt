--TEST--
Test Imagick::setImageArtifact and Imagick::getImageArtifact
--SKIPIF--
<?php

$imageMagickRequiredVersion = 0x656;

require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('setImageArtifact', 'getImageArtifact', 'deleteImageArtifact'));

?>
--FILE--
<?php

$im = new IMagick(__DIR__ . '/php.gif');

/* Examle from http://php.net/manual/de/imagick.setimageartifact.php */
var_dump($im->setImageArtifact('compose:args', '1,0,-0.5,0.5'));

var_dump($im->getImageArtifact('compose:args'));
var_dump($im->deleteImageArtifact('compose:args'));

?>
--EXPECT--
bool(true)
string(12) "1,0,-0.5,0.5"
bool(true)