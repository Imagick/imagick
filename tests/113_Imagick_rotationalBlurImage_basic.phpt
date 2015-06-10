--TEST--
Test Imagick, rotationalBlurImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); 

checkClassMethods('Imagick', array('rotationalBlurImage'));

?>
--FILE--
<?php


function rotationalBlurImage() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");
    $imagick->rotationalBlurImage(3);
    $imagick->rotationalBlurImage(5);
    $imagick->rotationalBlurImage(7);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

rotationalBlurImage() ;
echo "Ok";
?>
--EXPECTF--
Ok