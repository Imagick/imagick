<?
	$handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
	if ( imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

		print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}
	
	$cloned_handle = imagick_clonehandle( $handle ) ;
	imagick_scale( $cloned_handle, 640, 480 ) ;

        if ( !imagick_writeimage( $cloned_handle, getcwd() . "/new_image.tif" ) )
        {
                $reason      = imagick_failedreason( $cloned_handle ) ;
                $description = imagick_faileddescription( $cloned_handle ) ;

                print "imagick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

        print "Done!<BR>\n" ;
?>
