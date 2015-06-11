--TEST--
Test Imagick, distortImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
checkFormatPresent('png');
?>
--FILE--
<?php

$distortion = 1;

        //$imagick = new \Imagick(realpath($this->rsiControl->getImagePath()));
        $imagick = new \Imagick();

        /* Create new checkerboard pattern */
        $imagick->newPseudoImage(100, 100, "pattern:checkerboard");

        /* Set the image format to png */
        $imagick->setImageFormat('png');

        /* Fill new visible areas with transparent */
        $imagick->setImageVirtualPixelMethod(\Imagick::VIRTUALPIXELMETHOD_TRANSPARENT);

        /* Activate matte */
        $imagick->setImageMatte(true);

        /* Control points for the distortion */
        $controlPoints = array( 10, 10,
            10, 5,

            10, $imagick->getImageHeight() - 20,
            10, $imagick->getImageHeight() - 5,

            $imagick->getImageWidth() - 10, 10,
            $imagick->getImageWidth() - 10, 20,

            $imagick->getImageWidth() - 10, $imagick->getImageHeight() - 10,
            $imagick->getImageWidth() - 10, $imagick->getImageHeight() - 30);

        /* Perform the distortion */
        $imagick->distortImage(\Imagick::DISTORTION_PERSPECTIVE, $controlPoints, true);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

echo "Ok";
?>
--EXPECTF--
Ok