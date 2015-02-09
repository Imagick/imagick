--TEST--
Test ImagickDraw, bezier
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function bezier($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $strokeColor = new \ImagickPixel($strokeColor);
    $fillColor = new \ImagickPixel($fillColor);

    $draw->setStrokeOpacity(1);
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);

    $draw->setStrokeWidth(2);

    $smoothPointsSet = array(
        array(
            array('x' => 10.0 * 5, 'y' => 10.0 * 5),
            array('x' => 30.0 * 5, 'y' => 90.0 * 5),
            array('x' => 25.0 * 5, 'y' => 10.0 * 5),
            array('x' => 50.0 * 5, 'y' => 50.0 * 5),
        ),
        array(
            array('x' => 50.0 * 5, 'y' => 50.0 * 5),
            array('x' => 75.0 * 5, 'y' => 90.0 * 5),
            array('x' => 70.0 * 5, 'y' => 10.0 * 5),
            array('x' => 90.0 * 5, 'y' => 40.0 * 5),
        ),
    );

    foreach ($smoothPointsSet as $points) {
        $draw->bezier($points);
    }


    $disjointPoints = array(
        array(
            array('x' => 10 * 5, 'y' => 10 * 5),
            array('x' => 30 * 5, 'y' => 90 * 5),
            array('x' => 25 * 5, 'y' => 10 * 5),
            array('x' => 50 * 5, 'y' => 50 * 5),
        ),
        array(
            array('x' => 50 * 5, 'y' => 50 * 5),
            array('x' => 80 * 5, 'y' => 50 * 5),
            array('x' => 70 * 5, 'y' => 10 * 5),
            array('x' => 90 * 5, 'y' => 40 * 5),
        )
    );
    $draw->translate(0, 200);

    foreach ($disjointPoints as $points) {
        $draw->bezier($points);
    }

    //Create an image object which the draw commands can be rendered into
    $imagick = new \Imagick();
    $imagick->newImage(500, 500, $backgroundColor);
    $imagick->setImageFormat("png");

    //Render the draw commands in the ImagickDraw object 
    //into the image.
    $imagick->drawImage($draw);

    //Send the image to the browser
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

bezier($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok