<?
	$handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
	if ( imagick_iserror( $handle ) )
	{
		$reason      = imagick_failedreason( $handle ) ;
		$description = imagick_faileddescription( $handle ) ;

		print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	imagick_begindraw( $handle ) ;

	if ( !imagick_drawarc( $handle, 50, 50, 187.2, 237.6, 30, 180 ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

		print "imagick_drawarc() failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
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
