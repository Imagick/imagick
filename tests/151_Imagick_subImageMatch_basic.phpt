--TEST--
Test Imagick, subImageMatch
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
$imageMagickBelowVersion=0x700;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function subImageMatch() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->adaptiveResizeImage(100, 100, true);
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

    if ($similarity >= 0.558 && $similarity <= 0.559) {
        echo "Similarity in range\n";
    }
    else {
        echo "Similarity is out of range.\n";
        var_dump($similarity);
    }
    
    ksort($bestMatch);
    foreach ($bestMatch as $key => $value) {
        echo "$key : $value\n";
    }
}

subImageMatch() ;
echo "Ok";
?>
--EXPECTF--
Similarity in range
height : 25
width : 25
x : 50
y : 50
Ok