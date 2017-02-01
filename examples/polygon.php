<?php
/*
	This examples was ported from ImageMagick C examples.
*/

/* Create Imagick objects */
$Imagick = new Imagick();

/* Create ImagickDraw objects */
$ImagickDraw = new ImagickDraw();

/* Create ImagickPixel objects */
$ImagickPixel = new ImagickPixel();

/* This array contains polygon geometry */
$array = array( array( "x" => 378.1, "y" => 81.72 ),
                array( "x" => 381.1, "y" => 79.56 ),
                array( "x" => 384.3, "y" => 78.12 ),
                array( "x" => 387.6, "y" => 77.33 ),
                array( "x" => 391.1, "y" => 77.11 ),
                array( "x" => 394.6, "y" => 77.62 ),
                array( "x" => 397.8, "y" => 78.77 ),
                array( "x" => 400.9, "y" => 80.57 ),
                array( "x" => 403.6, "y" => 83.02 ),
                array( "x" => 523.9, "y" => 216.8 ),
                array( "x" => 526.2, "y" => 219.7 ),
                array( "x" => 527.6, "y" => 223 ),
                array( "x" => 528.4, "y" => 226.4 ),
                array( "x" => 528.6, "y" => 229.8 ),
                array( "x" => 528.0, "y" => 233.3 ),
                array( "x" => 526.9, "y" => 236.5 ),
                array( "x" => 525.1, "y" => 239.5 ),
                array( "x" => 522.6, "y" => 242.2 ),
                array( "x" => 495.9, "y" => 266.3 ),
                array( "x" => 493, "y" => 268.5 ),
                array( "x" => 489.7, "y" => 269.9 ),
                array( "x" => 486.4, "y" => 270.8 ),
                array( "x" => 482.9, "y" => 270.9 ),
                array( "x" => 479.5, "y" => 270.4 ),
                array( "x" => 476.2, "y" => 269.3 ),
                array( "x" => 473.2, "y" => 267.5 ),
                array( "x" => 470.4, "y" => 265 ),
                array( "x" => 350, "y" => 131.2 ),
                array( "x" => 347.8, "y" => 128.3 ),
                array( "x" => 346.4, "y" => 125.1 ),
                array( "x" => 345.6, "y" => 121.7 ),
                array( "x" => 345.4, "y" => 118.2 ),
                array( "x" => 346, "y" => 114.8 ),
                array( "x" => 347.1, "y" => 111.5 ),
                array( "x" => 348.9, "y" => 108.5 ),
                array( "x" => 351.4, "y" => 105.8 ),
                array( "x" => 378.1, "y" => 81.72 ),
              );

/* This ImagickPixel is used to set background color */
$ImagickPixel->setColor( 'gray' );

/* Create new image, set color to gray and format to png*/
$Imagick->newImage( 700, 500, $ImagickPixel );
$Imagick->setImageFormat( 'png' );

/* Create the polygon*/
$ImagickDraw->polygon( $array );

/* Render the polygon to image*/
$Imagick->drawImage( $ImagickDraw );

/* Send headers and output the image */
header( "Content-Type: image/{$Imagick->getImageFormat()}" );
echo $Imagick->getImageBlob( );

?>
