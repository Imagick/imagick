<?
	$handle = imagick_readimage( "./image.jpg" ) ;
	if ( imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

		print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	$colorspace = imagick_getcolorspace( $handle ) ;
	if ( $colorspace == IMAGICK_COLORSPACE_RGB )
	{
		print "RGB Colorspace!<BR>" ;
	}
	/*
	else if ( $colorspace == IMAGICK_COLORSPACE_* )
	{
		print "[The text colorspace]!<BR>" ;
	}
	else if ()
	{
	}
	...
	*/
?>
