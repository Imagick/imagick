<?php

/*
        A simple example demonstrate houghline creation.
*/

/* Create the Imagick object */
$im = new Imagick();

/* Read the image file */
$im->readImage( '/tmp/test.png' );

/* Thumbnail the image ( width 100, preserve dimensions ) */
$im->houghLineImage( 9, 9, 40 );

/* Write the thumbail to disk */
$im->writeImage( '/tmp/hough_test.png' );

/* Free resources associated to the Imagick object */
$im->destroy();

?>

