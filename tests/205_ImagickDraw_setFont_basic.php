<?php

$backgroundColor = 'rgb(225, 225, 225)';
$strokeColor = 'rgb(0, 0, 0)';
$fillColor = 'DodgerBlue2';

function setFont($fillColor, $strokeColor, $backgroundColor) {

    $draw = new \ImagickDraw();

    $draw->setStrokeColor($strokeColor);
    $draw->setFillColor($fillColor);

    $draw->setStrokeWidth(2);
    $draw->setFontSize(36);

    $draw->annotation(50, 50, "Lorem Ipsum!");

    $draw->setFont("../fonts/Consolas.ttf");
    $draw->annotation(50, 100, "Lorem Ipsum!");

    $draw->annotation(50, 150, "Lorem Ipsum!");

    $draw->setFont("../fonts/Inconsolata-dz.otf");
    $draw->annotation(50, 200, "Lorem Ipsum!");

    $imagick = new \Imagick();
    $imagick->newImage(500, 300, $backgroundColor);
    $imagick->setImageFormat("png");
    $imagick->drawImage($draw);

    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

setFont($fillColor, $strokeColor, $backgroundColor) ;
echo "Ok";
?>
