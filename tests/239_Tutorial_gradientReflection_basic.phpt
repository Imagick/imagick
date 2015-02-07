--TEST--
Test Tutorial, gradientReflection
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php


function gradientReflection() {
    $im = new \Imagick();
    $im->newPseudoImage(640, 480, "magick:logo");
    
    $reflection = clone $im;

    $reflection->flipImage();

    $reflection->cropImage($im->getImageWidth(), $im->getImageHeight() * 0.75, 0, 0);

    $gradient = new \Imagick();
    $gradient->newPseudoImage(
         $reflection->getImageWidth(),
         $reflection->getImageHeight(),
         //Putting spaces in the rgba string is bad
         'gradient:rgba(255,0,255,0.6)-rgba(255,255,0,0.99)'
    );

    $reflection->compositeimage(
       $gradient,
       \Imagick::COMPOSITE_DSTOUT,
       0, 0
    );

    $canvas = new \Imagick();
    $canvas->newImage($im->getImageWidth(), $im->getImageHeight() * 1.75, new \ImagickPixel('rgba(255, 255, 255, 0)'));
    $canvas->compositeImage($im, \Imagick::COMPOSITE_BLEND, 0, 0);
    $canvas->setImageFormat('png');
    $canvas->compositeImage($reflection, \Imagick::COMPOSITE_BLEND, 0, $im->getImageHeight());

    $canvas->stripImage();
    $canvas->setImageFormat('png');
    header('Content-Type: image/png');
    $bytes = $canvas;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

gradientReflection() ;
echo "Ok";
?>
--EXPECTF--
Ok