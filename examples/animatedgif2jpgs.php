<?
        $handle = imagick_readimage( "./animated.gif" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

        if ( !imagick_writeimages( $handle, "./new_image.jpg" ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_writeimages() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

        print "Done!<BR>\n" ;
?>
