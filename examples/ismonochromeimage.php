<?
        $handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

	if ( !imagick_ismonochromeimage( $handle ) )
	{
		print "The image is not a monochrome image!<BR>\n" ;
	}
	else
	{
		print "The image is a monochrome image!<BR>\n" ;
	}
?>
