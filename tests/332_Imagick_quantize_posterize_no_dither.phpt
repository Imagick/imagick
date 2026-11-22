--TEST--
Test Imagick, quantizeImage, quantizeImages and posterizeImage do not dither when dither is false
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

function gradient() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(64, 1, "gradient:black-white");
    $imagick->setImageType(\Imagick::IMGTYPE_GRAYSCALE);
    return $imagick;
}

function reversals(\Imagick $imagick) {
    $pixels = $imagick->exportImagePixels(0, 0, 64, 1, "R", \Imagick::PIXEL_CHAR);
    $count = 0;
    for ($i = 1; $i < count($pixels); $i++) {
        if ($pixels[$i] < $pixels[$i - 1]) {
            $count++;
        }
    }
    return $count;
}

$imagick = gradient();
$imagick->quantizeImage(2, \Imagick::COLORSPACE_GRAY, 0, false, false);
echo "quantizeImage: " . reversals($imagick) . "\n";

$imagick = gradient();
$imagick->quantizeImages(2, \Imagick::COLORSPACE_GRAY, 0, false, false);
echo "quantizeImages: " . reversals($imagick) . "\n";

$imagick = gradient();
$imagick->posterizeImage(2, false);
echo "posterizeImage: " . reversals($imagick) . "\n";

echo "Ok";
?>
--EXPECT--
quantizeImage: 0
quantizeImages: 0
posterizeImage: 0
Ok
