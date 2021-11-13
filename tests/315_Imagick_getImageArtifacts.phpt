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

    foreach ($artifacts as $key => $value) {
        if ($value === null) {
            echo "$key is null\n";
        }
        else {
            echo "$key = $value\n";
            }
    }

    $imagick->getImageBlob();
}

getImageArtifacts();
echo "Ok";
?>
--EXPECTF--
aux:Firmware is null
aux:FlashCompensation is null
aux:ImageNumber is null
aux:Lens is null
aux:LensID is null
aux:SerialNumber is null
date:create is null
date:modify is null
exif:ApertureValue is null
exif:BodySerialNumber is null
exif:ColorSpace is null
exif:CustomRendered is null
exif:DateTime is null
exif:DateTimeDigitized is null
exif:DateTimeOriginal is null
exif:ExifOffset is null
exif:ExifVersion is null
exif:ExposureBiasValue is null
exif:ExposureMode is null
exif:ExposureProgram is null
exif:ExposureTime is null
exif:Flash is null
exif:FNumber is null
exif:FocalLength is null
exif:FocalPlaneResolutionUnit is null
exif:FocalPlaneXResolution is null
exif:FocalPlaneYResolution is null
exif:LensModel is null
exif:LensSpecification is null
exif:Make is null
exif:MaxApertureValue is null
exif:MeteringMode is null
exif:Model is null
exif:Orientation is null
exif:PhotographicSensitivity is null
exif:PixelXDimension is null
exif:PixelYDimension is null
exif:ResolutionUnit is null
exif:SceneCaptureType is null
exif:ShutterSpeedValue is null
exif:Software is null
exif:WhiteBalance is null
exif:XResolution is null
exif:YResolution is null
jpeg:colorspace is null
jpeg:sampling-factor is null
photoshop:DateCreated is null
Ok
