--TEST--
Test ImagickDraw:: setTextInterlineSpacing
--SKIPIF--
<?php
$minimumVersions = ['6.9.8-6', '7.0.5-7'];
require_once(dirname(__FILE__) . '/skipif.inc');

?>
--FILE--
<?php

require_once(dirname(__FILE__) . '/functions.inc');

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

$interlineSpacings = [0, 16, 24, 36];

$imageHeights = [];

foreach ($interlineSpacings as $interlineSpacing) {

    $draw = new \ImagickDraw();

    setFontForImagickDraw($draw);

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);

    $draw->setStrokeWidth(2);
    $draw->setFontSize(56);

    $draw->setFontSize(16);
    $draw->setStrokeAntialias(true);
    $draw->setTextAntialias(true);
    $draw->setFillColor('#ff0000');
    $draw->setTextInterlineSpacing($interlineSpacing);

    $imagick = new \Imagick();
    $imagick->newImage(600, 600, "rgb(230, 230, 230)");
    $imagick->setImageFormat('png');
    $imagick->annotateImage($draw, 30, 40, 0, "Line 1\nLine 2\nLine 3");
    $imagick->trimImage(0);
    $imagick->setImagePage($imagick->getimageWidth(), $imagick->getimageheight(), 0, 0);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) {
        die("Failed to generate image.");
    }

    $imageHeights[$interlineSpacing] = $imagick->getImageHeight();

    $imagick->writeImage(__DIR__ . "/interline_spacing_test_$interlineSpacing.png");
}


$previousHeight = null;

foreach ($imageHeights as $interlineSpacing => $imageHeight) {
    if ($previousHeight !== null) {
        $differenceFromPrevious = $imageHeight - $previousHeight;
        if ($differenceFromPrevious < 15) {
            echo "textInterlineSpacing of $interlineSpacing only resulted in extra height of $differenceFromPrevious\n";
        }
    }

    $previousHeight = $imageHeight;
}

echo "Ok";
?>
--CLEAN--
<?php
$interlineSpacings = [0, 16, 24, 36];
foreach ($interlineSpacings as $interlineSpacing) {
    @unlink(__DIR__ . "/interline_spacing_test_$interlineSpacing.png");
}

?>
--EXPECTF--
Ok
