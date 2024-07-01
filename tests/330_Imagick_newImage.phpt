--TEST--
Test Imagick, newImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');

?>
--FILE--
<?php


$image = new Imagick();
$image->newImage(100, 100, new ImagickPixel('red'), 'png');
echo $image->getImageFormat();
echo "\n";

$image = new Imagick();
$image->newImage(100, 100, 'blue', null);

try {
    $image->getImageFormat();
}
catch (ImagickException $ie) {
    echo $ie->getMessage() . "\n";
}

echo "Ok";
?>
--EXPECTF--
PNG
Image has no format
Ok
