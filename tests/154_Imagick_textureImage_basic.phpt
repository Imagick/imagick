--TEST--
Test Imagick, textureImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php


function textureImage() {
    $image = new \Imagick();
    $image->newImage(640, 480, new \ImagickPixel('pink'));
    $image->setImageFormat("jpg");
    
    $texture = new \Imagick();
    $texture->newPseudoImage(640, 480, "magick:logo");
    $texture->scaleimage($image->getimagewidth() / 4, $image->getimageheight() / 4);
    $image = $image->textureImage($texture);
    $bytes = $image;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

textureImage() ;
echo "Ok";
?>
--EXPECTF--
Ok