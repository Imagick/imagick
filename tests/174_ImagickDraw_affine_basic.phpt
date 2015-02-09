--TEST--
Test ImagickDraw, affine
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function affine($strokeColor, $fillColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeWidth(1);
    $draw->setStrokeOpacity(1);
    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);

    $draw->setStrokeWidth(2);

    $PI = 3.141592653589794;
    $angle = 60 * $PI / 360;

    //Scale the drawing co-ordinates.
    $affineScale = array("sx" => 1.75, "sy" => 1.75, "rx" => 0, "ry" => 0, "tx" => 0, "ty" => 0);

    //Shear the drawing co-ordinates.
    $affineShear = array("sx" => 1, "sy" => 1, "rx" => sin($angle), "ry" => -sin($angle), "tx" => 0, "ty" => 0);

    //Rotate the drawing co-ordinates. The shear affine matrix
    //produces incorrectly scaled drawings.
    $affineRotate = array("sx" => cos($angle), "sy" => cos($angle), "rx" => sin($angle), "ry" => -sin($angle), "tx" => 0, "ty" => 0,);

    //Translate (offset) the drawing
    $affineTranslate = array("sx" => 1, "sy" => 1, "rx" => 0, "ry" => 0, "tx" => 30, "ty" => 30);

    //The identiy affine matrix
    $affineIdentity = array("sx" => 1, "sy" => 1, "rx" => 0, "ry" => 0, "tx" => 0, "ty" => 0);

    $examples = array($affineScale, $affineShear, $affineRotate, $affineTranslate, $affineIdentity,);

    $count = 0;

    foreach ($examples as $example) {
        $draw->push();
        $draw->translate(($count % 2) * 250, intval($count / 2) * 250);
        $draw->translate(100, 100);
        $draw->affine($example);
        $draw->rectangle(-50, -50, 50, 50);
        $draw->pop();
        $count++;
    }

    //Create an image object which the draw commands can be rendered into
    $image = new \Imagick();
    $image->newImage(500, 750, $backgroundColor);
    $image->setImageFormat("png");

    //Render the draw commands in the ImagickDraw object 
    //into the image.
    $image->drawImage($draw);

    //Send the image to the browser
    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

affine($strokeColor, $fillColor, $backgroundColor) ;
echo "Ok";
?>
--EXPECTF--
Ok