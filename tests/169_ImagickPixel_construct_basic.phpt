--TEST--
Test ImagickPixel, construct
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php


function construct() {

    $columns = 4;
    
    $exampleColors = array(
        "rgba(100%, 0%, 0%, 0.5)",
        "hsb(33.3333%, 100%,  75%)", // medium green
        "hsl(120, 255,   191.25)", //medium green
        "graya(50%, 0.5)", //  semi-transparent mid gray
        "LightCoral", "none", //"cmyk(0.9, 0.48, 0.83, 0.50)",
        "#f00", //  #rgb
        "#ff0000", //  #rrggbb
        "#ff0000ff", //  #rrggbbaa
        "#ffff00000000", //  #rrrrggggbbbb
        "#ffff00000000ffff", //  #rrrrggggbbbbaaaa
        "rgb(255, 0, 0)", //  an integer in the range 0—255 for each component
        "rgb(100.0%, 0.0%, 0.0%)", //  a float in the range 0—100% for each component
        "rgb(255, 0, 0)", //  range 0 - 255
        "rgba(255, 0, 0, 1.0)", //  the same, with an explicit alpha value
        "rgb(100%, 0%, 0%)", //  range 0.0% - 100.0%
        "rgba(100%, 0%, 0%, 1.0)", //  the same, with an explicit alpha value
    );

    $draw = new \ImagickDraw();
    $count = 0;
    $black = new \ImagickPixel('rgb(0, 0, 0)');

    foreach ($exampleColors as $exampleColor) {
        $color = new \ImagickPixel($exampleColor);

        $draw->setstrokewidth(1.0);
        $draw->setStrokeColor($black);
        $draw->setFillColor($color);
        $offsetX = ($count % $columns) * 50 + 5;
        $offsetY = intval($count / $columns) * 50 + 5;
        $draw->rectangle(0 + $offsetX, 0 + $offsetY, 40 + $offsetX, 40 + $offsetY);
        $count++;
    }

    $image = new \Imagick();
    $image->newImage(350, 350, "blue");
    $image->setImageFormat("png");
    $image->drawImage($draw);
    $bytes = $image->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

construct() ;
echo "Ok";
?>
--EXPECTF--
Ok