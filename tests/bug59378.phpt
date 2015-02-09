--TEST--
Test PHP bug #59378 writing to php://memory is incomplete
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$imagick = new Imagick();
$imagick->newPseudoImage(640, 480, "LOGO:");
$imagick->setFormat("png");

$fp = fopen("php://memory", 'r+');
$imagick->writeImageFile($fp);
rewind($fp);
$memoryBlob = stream_get_contents($fp);
fclose($fp);

//This test depends on getImageBlob working correctly.
$imageBlob = $imagick->getImageBlob();

//Read the images from the data blobs.
$imageReopened = new Imagick();
$imageReopened->readImageBlob($imageBlob);
$memoryReopened = new Imagick();
$memoryReopened->readImageBlob($memoryBlob);

//Compare to see if they are identical.
$result = $imageReopened->compareImages($memoryReopened, \Imagick::METRIC_MEANABSOLUTEERROR);

if ($result[1] == 0) {
    echo "Reopened images are identical.";
}
else {
    echo "Error, reopened images have changed.";
    var_dump($result);
}

?>
--EXPECTF--
Reopened images are identical.
