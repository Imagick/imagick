--TEST--
Test whether ImageMagick is producing valid images.
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
//checkClassMethods('Imagick', array('averageImages'));
?>
--FILE--
<?php

function testOpacity() {
    $imagick = new \Imagick();
    $imagick->newPseudoImage(640, 480, "magick:LOGO");

    $transparency = new Imagick();
    $transparency->newPseudoImage(640, 480, "xc:rgba(0,0,0,0)");

    $draw = new \ImagickDraw();
    $draw->setFillColor('white');
    $draw->setStrokeColor('white');

    $draw->circle(
        320,
        240,
        500,
        240
    );

    $transparency->drawImage($draw);

    $transparency->blurImage(50, 15);
    $transparency->writeImage(__DIR__ . "/304_output_trans.png");

    $checkerboard = new Imagick();
    $checkerboard->newPseudoImage(640, 480, "pattern:checkerboard");
    $checkerboard->setImageFormat('png');

    $output = clone $checkerboard;
    $canvas = clone $imagick;
    $canvas->compositeImage($transparency, \Imagick::COMPOSITE_COPYOPACITY, 0, 0);
    $output->compositeImage($canvas, \Imagick::COMPOSITE_ATOP, 0, 0);
    $output->writeImage(__DIR__ . "/304_output_before_softlight.png");

    $output = clone $checkerboard;
    $gradient = new Imagick();
    $gradient->newPseudoImage(640, 480, 'gradient:#979797-#373737');
    $canvas->compositeImage($gradient, Imagick::COMPOSITE_SOFTLIGHT, 0, 0);

    $output->compositeImage($canvas, \Imagick::COMPOSITE_ATOP, 0, 0);
    // TODO - we need to store a known good output and compare
    // the result image against that.
//    $output->writeImage(__DIR__ . "/304_output_with_softlight.png");
}

testOpacity() ;
echo "Ok";
?>
--CLEAN--
<?php
@unlink(__DIR__ . "/304_output_trans.png");
@unlink(__DIR__ . "/304_output_before_softlight.png");
?>
--EXPECTF--
Ok
