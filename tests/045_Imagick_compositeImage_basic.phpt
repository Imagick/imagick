--TEST--
Test Imagick, compositeImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function compositeImage() {

    $img1 = new \Imagick();
    $img1->newPseudoImage(640, 480, "magick:logo");

    $img2 = new \Imagick();
    $img2->newPseudoImage(640, 480, "magick:logo"); 
    $img2->negateImage(false);
    $img2->blurimage(10, 5);


    $img1->resizeimage(
        $img2->getImageWidth(),
        $img2->getImageHeight(),
        \Imagick::FILTER_LANCZOS,
        1
    );

    $opacity = new \Imagick();    
    $opacity->newPseudoImage(
        $img1->getImageHeight(),
        $img1->getImageWidth(),
        "gradient:gray(10%)-gray(90%)"
    );
    $opacity->rotateimage('black', 90);

    $img2->compositeImage($opacity, \Imagick::COMPOSITE_COPYOPACITY, 0, 0);
    $img1->compositeImage($img2, \Imagick::COMPOSITE_ATOP, 0, 0);

    $bytes = $img1->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

compositeImage() ;
echo "Ok";
?>
--EXPECTF--
Ok