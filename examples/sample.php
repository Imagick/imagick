<?
	$handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
	if ( imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

		print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	if ( !imagick_sample( $handle, 200, 200, "!" ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "imagick_sample() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
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
