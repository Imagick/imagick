--TEST--
Test Imagick, forwardFourierTransformImage
--SKIPIF--
<?php 
require_once(dirname(__FILE__) . '/skipif.inc');
require_once(dirname(__FILE__) . '/skipprobefourier.inc');
?>
--FILE--
<?php


//Utility function for forwardTransformImage
function createMask() {
    $draw = new \ImagickDraw();

    $draw->setStrokeOpacity(0);
    $draw->setStrokeColor('rgb(255, 255, 255)');
    $draw->setFillColor('rgb(255, 255, 255)');

    //Draw a circle on the y-axis, with it's centre
    //at x, y that touches the origin
    $draw->circle(250, 250, 220, 250);

    $imagick = new \Imagick();
    $imagick->newImage(512, 512, "black");
    $imagick->drawImage($draw);
    $imagick->gaussianBlurImage(20, 20);
    $imagick->autoLevelImage();

    return $imagick;
}


function forwardFourierTransformImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->resizeimage(512, 512, \Imagick::FILTER_LANCZOS, 1);

    $mask = createMask();
    $imagick->forwardFourierTransformImage(true);

    $imagick->setIteratorIndex(0);
    $magnitude = $imagick->getimage();

    $imagick->setIteratorIndex(1);
    $imagickPhase = $imagick->getimage();

    if (true) {
        $imagickPhase->compositeImage($mask, \Imagick::COMPOSITE_MULTIPLY, 0, 0);
    }

    if (false) {
        $output = clone $imagickPhase;
        $output->setimageformat('png');
    $bytes = $output->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
    }

    $magnitude->inverseFourierTransformImage($imagickPhase, true);

    $magnitude->setimageformat('png');
    $bytes = $magnitude->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

forwardFourierTransformImage() ;
echo "Ok";
?>
--EXPECTF--
Ok