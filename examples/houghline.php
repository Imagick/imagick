<?php

/*
        A simple example demonstrate houghline creation.
*/

/* Create the Imagick object */
$im = new Imagick();

/* Read the image file */
$im->readImage( '/tmp/test.png' );

/* Houghline the image ( width 9, height 9, threshold 40 ) */
$im->houghLineImage( 9, 9, 40 );

/* Write the houghline image to disk */
$im->writeImage( '/tmp/hough_test.png' );

/* Free resources associated to the Imagick object */
$im->destroy();

?>

