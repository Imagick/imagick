--TEST--
Test Imagick, statisticImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); 

checkClassMethods('Imagick', array('statisticImage'));

?>
--FILE--
<?php

$statisticType = 1;
$w20 = 5;
$h20 = 5;
$channel = Imagick::CHANNEL_DEFAULT;

function statisticImage($statisticType, $w20, $h20, $channel) {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:logo");

    $imagick->statisticImage(
        $statisticType,
        $w20,
        $h20,
        $channel
    );

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

statisticImage($statisticType, $w20, $h20, $channel) ;
echo "Ok";
?>
--EXPECTF--
Ok