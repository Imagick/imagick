--TEST--
Test Imagick, getImageArtifacts
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('getImageArtifacts'));
?>
--FILE--
<?php

function getImageArtifacts() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $artifacts = $imagick->getImageArtifacts();

// TODO - get a better test image with artifact data in it.
//
//    $expectedEntries = [
//        "exif:ApertureValue" => false,
//        "exif:ColorSpace" => false,
//        "exif:CustomRendered" => false,
//        "exif:DateTime" => false,
//        "exif:DateTimeDigitized" => false,
//        "exif:DateTimeOriginal" => false,
//        "exif:ExifOffset" => false,
//        "exif:ExifVersion" => false,
//    ];
//
//    foreach ($artifacts as $key => $value) {
//       if (array_key_exists($key, $expectedEntries) === true) {
//            $expectedEntries[$key] = true;
//        }
//    }
//
//    $any_failed = false;
//    foreach ($expectedEntries as $key => $value) {
//        if ($value !== true) {
//            echo "Expected entry $key was not set\n";
//            $any_failed = true;
//        }
//    }

//    if ($any_failed === true) {
//        var_dump($artifacts);
//    }

    $imagick->getImageBlob();
}

getImageArtifacts();
echo "Ok";
?>
--EXPECTF--
Ok
