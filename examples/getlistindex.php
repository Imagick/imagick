<?
        $handle = imagick_readimage( getcwd() . "/animated.gif" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

	imagick_next( $handle ) ;
	imagick_next( $handle ) ;
	imagick_next( $handle ) ;

	print "You are frame #" . imagick_getlistindex( $handle ) .
	      "<BR>\n" ;
?>
