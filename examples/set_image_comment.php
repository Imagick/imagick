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
	//  When setting a comment, you should clear it first then reset it.
	//  Just adding a comment will append to any existing comment - which
	//  might not be what you want.
	//
	imagick_set_image_comment( $handle, NULL ) ;
	imagick_set_image_comment( $handle, "Hello World!" ) ;

        if ( !imagick_writeimage( $handle, "./new_image.jpg" ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "magick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

	header( "Location: images_jpg.html" ) ;
?>
