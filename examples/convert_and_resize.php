<?
	$handle = imagick_readimage( "./some_image_that_doesnt_exist.jpg" ) ;
	if ( imagick_iserror( $handle ) )
	{
		//
		//  This should fail!  Notice the image name is spelled
		//  incorrectly.  This is to demonstrate error reporting.
		//

		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	$handle = imagick_readimage( "./image.jpg" ) ;
	if ( imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

		print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	imagick_convert( $handle, "GIF" ) ;

	if ( !imagick_resize( $handle, 800, 600, IMAGICK_FILTER_QUADRATIC, 0 ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "imagick_resize() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	if ( !imagick_magnify( $handle ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "imagick_magnify() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	if ( !imagick_minify( $handle ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "imagick_minify() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	if ( !imagick_sample( $handle, 300, 300 ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "imagick_sample() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	if ( !imagick_writeimage( $handle, "./new_image.gif" ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "imagick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

        print "Done!<BR>\n" ;
?>
