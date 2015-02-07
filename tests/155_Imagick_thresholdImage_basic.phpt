--TEST--
Test Imagick, thresholdImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$threshold = 0.5;
$channel = Imagick::CHANNEL_DEFAULT;

function thresholdimage($threshold, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->thresholdimage($threshold * \Imagick::getQuantum(), $channel);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

thresholdimage($threshold, $channel) ;
echo "Ok";
?>
--EXPECTF--
Ok