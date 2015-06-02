--TEST--
Test for round issues
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$imagick = new Imagick();
$imagick->newImage(1128, 1128, '#dddddd', 'jpg' );
$imagick->cropThumbnailImage(250, 250);

$size = $imagick->getImageGeometry();

var_dump($size);

?>
--EXPECTF--
array(2) {
  ["width"]=>
  int(250)
  ["height"]=>
  int(250)
}
