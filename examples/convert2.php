<?
        $handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

	imagick_convert( $handle, "GIF" ) ;

	//
	//  I intentionally left the .gif extension off the file name to
	//  demonstrate a second way to convert images.  You might use
	//  this method when reading an image from a file, converting it
	//  and then immediately displaying it to the user.
	//

        if ( !imagick_writeimage( $handle, getcwd() . "/new_image" ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

        print "Done!<BR>\n" ;
?>
