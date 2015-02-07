--TEST--
Test Tutorial, svgExample
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php


function svgExample() {

    $svg = <<< END
<?xml version="1.0"?>
<svg version='1.0' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='746' height='742' viewBox='-362 -388 746 742' encoding='UTF-8' standalone='no'>
    <defs>
        <ellipse id='ellipse' cx='36' cy='-56' rx='160' ry='320' />
        <g id='ellipses'>
            <use xlink:href='#ellipse' fill='#0000ff' />
            <use xlink:href='#ellipse' fill='#0099ff' transform='rotate(72)' />
        </g>
    </defs>
</svg>
END;

    
    
    $svg = '<?xml version="1.0"?>
    <svg width="120" height="120"
         viewPort="0 0 120 120" version="1.1"
         xmlns="http://www.w3.org/2000/svg">

        <defs>
            <clipPath id="myClip">
                <circle cx="30" cy="30" r="20"/>
                <circle cx="70" cy="70" r="20"/>
            </clipPath>
        </defs>

        <rect x="10" y="10" width="100" height="100"
              clip-path="url(#myClip)"/>

    </svg>';



    
    $image = new \Imagick();

    $image->readImageBlob($svg);
    $image->setImageFormat("jpg");
    $bytes = $image;
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
}

svgExample() ;
echo "Ok";
?>
--EXPECTF--
Ok