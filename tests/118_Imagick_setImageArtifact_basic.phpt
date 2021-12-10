--TEST--
Test Imagick, setImageArtifact
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php


function setImageArtifact() {
    $src1 = new \Imagick();
    $src1->newPseudoImage(640, 480, "magick:logo");
    
    $src2 = new \Imagick();
    $src2->newPseudoImage(480, 640, "magick:WIZARD");

    $compose_args = $src2->getImageArtifact('compose:args');
    var_dump($compose_args);

    $src2->setImageVirtualPixelMethod(\Imagick::VIRTUALPIXELMETHOD_TRANSPARENT);
    $src2->setImageArtifact('compose:args', "1,0,-0.5,0.5");
    $src1->compositeImage($src2, Imagick::COMPOSITE_MATHEMATICS, 0, 0);

    $compose_args = $src2->getImageArtifact('compose:args');
    var_dump($compose_args);

    $src2->setImageArtifact('compose:args', null);
    $compose_args2 = $src2->getImageArtifact('compose:args');
    var_dump($compose_args2);

    $src1->setImageFormat('png');
    $bytes = $src1->getImagesBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setImageArtifact() ;
echo "Ok";
?>
--EXPECTF--
NULL
string(12) "1,0,-0.5,0.5"
NULL
Ok