<?
        $handle = imagick_readimage( "./image.jpg" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

        $ref_handle = imagick_readimage( "./animated.gif" ) ;
        if ( imagick_iserror( $ref_handle ) )
        {
                $reason      = imagick_failedreason( $ref_handle ) ;
                $description = imagick_faileddescription( $ref_handle ) ;

                print "ref_handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

	print "Read 2 images...<BR>\n" ;

	if ( imagick_isimagesequal( $handle, $ref_handle ) )
	{
		print "They are equal!<BR>\n" ;
	}
	else
	{
		print "They are not equal!<BR>\n" ;
	}

	//
	//  The above images are not equal.  The 2 below are.
	//

	imagick_destroyhandle( $handle ) ;
	imagick_destroyhandle( $ref_handle ) ;

        $handle = imagick_readimage( "./image.jpg" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

        $ref_handle = imagick_readimage( "./image.jpg" ) ;
        if ( imagick_iserror( $ref_handle ) )
        {
                $reason      = imagick_failedreason( $ref_handle ) ;
                $description = imagick_faileddescription( $ref_handle ) ;

                print "ref_handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

	print "Read 2 images...<BR>\n" ;

	if ( imagick_isimagesequal( $handle, $ref_handle ) )
	{
		print "They are equal!<BR>\n" ;
	}
	else
	{
		print "They are not equal!<BR>\n" ;
	}
?>
