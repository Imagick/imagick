<?
        $handle = imagick_readimage( getcwd() . "/animated.gif" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle2 failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

	imagick_next( $handle ) ;
	imagick_next( $handle ) ;
	imagick_next( $handle ) ;
	imagick_next( $handle ) ;
	imagick_goto( $handle, 0 ) ;

	//
	//  Extract just the current image from the list.
	//

	$new_handle = imagick_getimagefromlist( $handle ) ;

	header( "Content-type: " . imagick_getmimetype( $new_handle ) ) ;
	print imagick_image2blob( $new_handle ) ;
?>
