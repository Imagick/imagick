<?
	$handle = imagick_readimage( getcwd() . "/some_image_that_doesnt_exist.jpg" ) ;
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

	$handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
	if ( imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

		print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	imagick_convert( $handle, "GIF" ) ;

        //
        //  For 4th parameter, use any of the following:
        //
        //      IMAGICK_FILTER_UNDEFINED
        //      IMAGICK_FILTER_POINT
        //      IMAGICK_FILTER_BOX
        //      IMAGICK_FILTER_TRIANGLE
        //      IMAGICK_FILTER_HERMITE
        //      IMAGICK_FILTER_HANNING
        //      IMAGICK_FILTER_HAMMING
        //      IMAGICK_FILTER_BLACKMAN
        //      IMAGICK_FILTER_GAUSSIAN
        //      IMAGICK_FILTER_QUADRATIC
        //      IMAGICK_FILTER_CUBIC
        //      IMAGICK_FILTER_CATROM
        //      IMAGICK_FILTER_MITCHELL
        //      IMAGICK_FILTER_LANCZOS
        //      IMAGICK_FILTER_BESSEL
        //      IMAGICK_FILTER_SINC
        //      IMAGICK_FILTER_UNKNOWN
        //
        //  Use IMAGICK_FILTER_UNKNOWN to defer to whatever filter is defined
        //  in the image.
        //

	if ( !imagick_resize( $handle, 800, 600, IMAGICK_FILTER_UNKNOWN, 0 ) )
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

	if ( !imagick_writeimage( $handle, getcwd() . "/new_image.gif" ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "imagick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

        print "Done!<BR>\n" ;
?>
