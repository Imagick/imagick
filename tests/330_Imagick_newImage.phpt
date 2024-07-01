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

// Image format case changed at some point in IM
echo strtolower($image->getImageFormat());
echo "\n";

$image = new Imagick();
$image->newImage(100, 100, 'blue', null);

try {
    $image->getImageFormat();
    echo "Failed to throw exception";
}
catch (ImagickException $ie) {
    echo $ie->getMessage() . "\n";
}

echo "Ok";
?>
--EXPECTF--
png
Image has no format
Ok
