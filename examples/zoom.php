<?
	$handle = imagick_readimage( "./image.jpg" ) ;
	if ( imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

		print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	//
	//  The difference between imagick_zoom() and imagick_resize() is
	//  that zoom applies the blur and filter that are defined by
	//  the image.  It is a much simpler way of resizing an image.
	//

	if ( !imagick_zoom( $handle, 700, 700, "!" ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "imagick_zoom() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	if ( !imagick_writeimage( $handle, "./new_image.jpg" ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "imagick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

        print "Done!<BR>\n" ;
?>
