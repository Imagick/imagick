<?
	$handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
	if ( imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

		print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	imagick_begindraw( $handle ) ;

        // 
        //  For available fonts, see the Name value in phpinfo() under the
        //  imagick section.
        //

        if ( !imagick_setfontface( $handle, "Palatino-Bold" ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_setfontface() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

	if ( !imagick_setfillcolor( $handle, "#990000" ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_setfontcolor() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
	}

	if ( !imagick_setfontsize( $handle, 16 ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_setfontsize() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	if ( !imagick_drawannotation( $handle, 325, 100, "Ahhhh, Cozumel!" ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_drawannotation() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

        if ( !imagick_writeimage( $handle, getcwd() . "/new_image.jpg" ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

        print "Done!<BR>\n" ;
?>
