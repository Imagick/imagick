<?
	if ( !( $handle = imagick_readimage( "./image.jpg" ) ) )
	{
		print "imagick_readimage() failed:<BR>\n" .
		      imagick_error() ;
		exit ;
	}

	imagick_set_image_quality( $handle, 75 ) ;

	if ( !imagick_writeimage( $handle, "./new_image.jpg" ) )
        {
		print "imagick_writeimage() failed:<BR>\n" .
		      imagick_error( $handle ) ;
		exit ;
        }

	header( "Location: images_jpg.html" ) ;
?>
