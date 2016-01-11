--TEST--
Test Imagick, subImageMatch
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

function subImageMatch() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->adaptiveResizeImage(100, 100, true);
    //This makes the test fall over on some versions of ImageMagick
    //$imagick->setImageAlphaChannel(\Imagick::ALPHACHANNEL_DEACTIVATE);
    $imagick2 = clone $imagick;
    $imagick2->cropimage(25, 25, 50, 50);
    $imagick2->setImageFormat('png');

    $imagick2->setImagePage(25, 25, 0, 0);
    $imagick2->vignetteimage(0.25, 0.75, 3, 3);

    $similarity = 'not set';
    $bestMatch = 'not st';
    $comparison = $imagick->subImageMatch($imagick2, $bestMatch, $similarity);

    $comparison->setImageFormat('png');
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

    $version = Imagick::getVersion();
    if ($version['versionNumber'] >= 0x700) {
        $expectedValue = 0.7642;
    }
    else {
        $expectedValue = 0.5585;
    }

    // This doesn't work reliably - the values are not comparable across versions
    // if (abs($expectedValue - $similarity) <= 0.005) {
    //     echo "Similarity in range\n";
    // }
    // else {
    //     echo "Similarity is out of range.\n";
    //     var_dump($similarity);
    // }
    
    ksort($bestMatch);
    foreach ($bestMatch as $key => $value) {
        echo "$key : $value\n";
    }
}

subImageMatch() ;
echo "Ok";
?>

--EXPECTF--
height : 25
width : 25
x : 50
y : 50
Ok