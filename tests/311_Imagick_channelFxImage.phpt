--TEST--
Test Imagick, channelFxImage
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('channelFxImage'));
?>
--FILE--
<?php

// MagickChannelFxImage() applies a channel expression to the specified image.
// The expression consists of one or more channels, either mnemonic or numeric
// (e.g. red, 1), separated by actions as follows:
//
//   <=>     exchange two channels (e.g. red<=>blue)
//   =>      transfer a channel to another (e.g. red=>green)
//   ,       separate channel operations (e.g. red, green)
//   |       read channels from next input image (e.g. red | green)
//   ;       write channels to next output image (e.g. red; green; blue)

function whiteBalanceImage() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $result = $imagick->channelFxImage("red<=>blue");
    //    $result->writeImage(__DIR__ . '/complexImages_output_image.png');

    $result->getImageBlob();
}

whiteBalanceImage() ;
echo "Ok";
?>
--EXPECTF--
Ok
