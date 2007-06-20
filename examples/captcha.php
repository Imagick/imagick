<?php

/*
	A very primitive captcha implementation
*/

/* Create Imagick object */
$Imagick = new Imagick();

/* Create the ImagickPixel object (used to set the background color on image) */
$bg = new ImagickPixel();

/* Set the pixel color to white */
$bg->setColor( 'white' );

/* Create a drawing object and set the font size */
$ImagickDraw = new ImagickDraw();

/* Set font and font size. You can also specify /path/to/font.ttf */
$ImagickDraw->setFont( 'Helvetica Regular' );
$ImagickDraw->setFontSize( 20 );

/* Create the text */
$alphanum = 'ABXZRMHTL23456789';
$string = substr( str_shuffle( $alphanum ), 2, 6 );

/* Create new empty image */
$Imagick->newImage( 85, 30, $bg ); 

/* Write the text on the image */
$Imagick->annotateImage( $ImagickDraw, 4, 20, 0, $string );

/* Add some swirl */
$Imagick->swirlImage( 20 );

/* Create a few random lines */
$ImagickDraw->line( rand( 0, 70 ), rand( 0, 30 ), rand( 0, 70 ), rand( 0, 30 ) );
$ImagickDraw->line( rand( 0, 70 ), rand( 0, 30 ), rand( 0, 70 ), rand( 0, 30 ) );
$ImagickDraw->line( rand( 0, 70 ), rand( 0, 30 ), rand( 0, 70 ), rand( 0, 30 ) );
$ImagickDraw->line( rand( 0, 70 ), rand( 0, 30 ), rand( 0, 70 ), rand( 0, 30 ) );
$ImagickDraw->line( rand( 0, 70 ), rand( 0, 30 ), rand( 0, 70 ), rand( 0, 30 ) );

/* Draw the ImagickDraw object contents to the image. */
$Imagick->drawImage( $ImagickDraw );

/* Give the image a format */
$Imagick->setImageFormat( 'png' );

/* Send headers and output the image */
header( "Content-Type: image/{$Imagick->getImageFormat()}" );
echo $Imagick->getImageBlob( );

?>