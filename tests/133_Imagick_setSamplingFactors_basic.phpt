--TEST--
Test Imagick, setSamplingFactors
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php


function setSamplingFactors() {

    $imagePath = "../imagick/images/FineDetail.png";
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->setImageFormat('jpg');
    $imagick->setSamplingFactors(array('2x2', '1x1', '1x1'));

    $compressed = $imagick->getImageBlob();

    
    $reopen = new \Imagick();
    $reopen->readImageBlob($compressed);

    $reopen->resizeImage(
        $reopen->getImageWidth() * 4,
        $reopen->getImageHeight() * 4,
        \Imagick::FILTER_POINT,
        1
    );
    
    $bytes = $reopen->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setSamplingFactors() ;
echo "Ok";
?>
--EXPECTF--
Ok