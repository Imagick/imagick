<?
	$handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
	if ( imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

		print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	//
	//  For 4th parameter, use any of the following:
	//
	//	IMAGICK_FILTER_UNDEFINED
	//	IMAGICK_FILTER_POINT
	//	IMAGICK_FILTER_BOX
	//	IMAGICK_FILTER_TRIANGLE
	//	IMAGICK_FILTER_HERMITE
	//	IMAGICK_FILTER_HANNING
	//	IMAGICK_FILTER_HAMMING
	//	IMAGICK_FILTER_BLACKMAN
	//	IMAGICK_FILTER_GAUSSIAN
	//	IMAGICK_FILTER_QUADRATIC
	//	IMAGICK_FILTER_CUBIC
	//	IMAGICK_FILTER_CATROM
	//	IMAGICK_FILTER_MITCHELL
	//	IMAGICK_FILTER_LANCZOS
	//	IMAGICK_FILTER_BESSEL
	//	IMAGICK_FILTER_SINC
	//	IMAGICK_FILTER_UNKNOWN
	//
	//  Use IMAGICK_FILTER_UNKNOWN to defer to whatever filter is defined
	//  in the image.
	//
	
	if ( !imagick_resize( $handle, 700, 700, IMAGICK_FILTER_UNKNOWN, 0,
		   	      "200+200!" ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "imagick_resize() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	if ( !imagick_writeimage( $handle, getcwd() . "/new_image.jpg" ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "imagick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

        print "Done!<BR>\n" ;
?>
