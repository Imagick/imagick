<?
        $handle = imagick_readimage( getcwd() . "/animated.gif" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle2 failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

	//
	//  Extract the first frame of the animted image to use as the
	//  composite image.
	//

	$comp_handle = imagick_getimagefromlist( $handle ) ;
	imagick_destroyhandle( $handle ) ;

        $handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle2 failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

	//
	//  Use any one of these for the 2nd argument:
	//
	//	IMAGICK_COMPOSITE_OP_OVER
	//	IMAGICK_COMPOSITE_OP_IN
	//	IMAGICK_COMPOSITE_OP_OUT
	//	IMAGICK_COMPOSITE_OP_ATOP
	//	IMAGICK_COMPOSITE_OP_XOR
	//	IMAGICK_COMPOSITE_OP_PLUS
	//	IMAGICK_COMPOSITE_OP_MINUS
	//	IMAGICK_COMPOSITE_OP_ADD
	//	IMAGICK_COMPOSITE_OP_SUBTRACT
	//	IMAGICK_COMPOSITE_OP_DIFFERENCE
	//	IMAGICK_COMPOSITE_OP_MULTIPLY
	//	IMAGICK_COMPOSITE_OP_BUMPMAP
	//	IMAGICK_COMPOSITE_OP_COPY
	//	IMAGICK_COMPOSITE_OP_COPYRED
	//	IMAGICK_COMPOSITE_OP_COPYGREEN
	//	IMAGICK_COMPOSITE_OP_COPYBLUE
	//	IMAGICK_COMPOSITE_OP_COPYOPACITY
	//	IMAGICK_COMPOSITE_OP_CLEAR
	//	IMAGICK_COMPOSITE_OP_DISSOLVE
	//	IMAGICK_COMPOSITE_OP_DISPLACE
	//	IMAGICK_COMPOSITE_OP_MODULATE
	//	IMAGICK_COMPOSITE_OP_THRESHOLD
	//	IMAGICK_COMPOSITE_OP_NONE
	//	IMAGICK_COMPOSITE_OP_DARKEN
	//	IMAGICK_COMPOSITE_OP_LIGHTEN
	//	IMAGICK_COMPOSITE_OP_HUE
	//	IMAGICK_COMPOSITE_OP_SATURATE
	//	IMAGICK_COMPOSITE_OP_COLORIZE
	//	IMAGICK_COMPOSITE_OP_LUMINIZE
	//	IMAGICK_COMPOSITE_OP_SCREEN
	//	IMAGICK_COMPOSITE_OP_OVERLAY
	//

	imagick_composite( $handle, IMAGICK_COMPOSITE_OP_OVER, $comp_handle,
	                   200, 200 ) ;

	//
	//  The image represented by handle now has the image from comp_handle
	//  placed on top of it at x 200, y 200.
	//

	if ( imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
	}

	if ( imagick_iserror( $comp_handle ) )
	{
                $reason      = imagick_failedreason( $comp_handle ) ;
                $description = imagick_faileddescription( $comp_handle ) ;

                print "comp_handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
	}

error_log( "writing!" ) ;
        if ( !imagick_writeimage( $handle, getcwd() . "/new_image.jpg" ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

        print "Done!<BR>\n" ;
?>
