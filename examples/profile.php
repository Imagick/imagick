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
	//  4th parameter is optional.  If left out, the named profile
	//  (specified by 2nd parameter) is removed.  If 2nd parameter is 
	//  *, the 4th parameter will apply to all profiles.  3rd parameter
	//  should be any of IMAGICK_PROFILE_*.  The currently supported ones
	//  are:
	//
	// 	IMAGICK_PROFILE_OWN		value=0
	//	IMAGICK_PROFILE_COPY		value=1
	//
	//  For better documentation, go here:
	//	http://www.imagemagick.org/www/api/transform.html#profileimage
	//

	imagick_profile( $handle, "*", IMAGICK_PROFILE_OWN ) ;
	if ( imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_profile() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
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
