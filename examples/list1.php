<?
        $handle = imagick_readimage( getcwd() . "/animated.gif" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle2 failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

	$new_handle = imagick_clonehandle( $handle ) ;
	imagick_next( $new_handle ) ;
	imagick_next( $new_handle ) ;
	imagick_prev( $new_handle ) ;
	imagick_prev( $new_handle ) ;

	header( "Content-type: " . imagick_getmimetype( $new_handle ) ) ;
	print imagick_image2blob( $new_handle ) ;
?>
