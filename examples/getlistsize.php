<?
        $handle = imagick_readimage( getcwd() . "/animated.gif" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

	print "Number of frames in image: " . imagick_getlistsize( $handle ) .
	      "<BR>\n" ;
?>
