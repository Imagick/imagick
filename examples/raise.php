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
	//  The last parameter determines if the image should looked raised
	//  or lowered.  A non-zero value will raise the image, 0 will
	//  lower it.
	//

	if ( !imagick_raise( $handle, 0, 0, 100, 100, 0 ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_raise() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
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
