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
        if ( !imagick_setfillcolor( $handle, "#ff0000" ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_setfillcolor() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	if ( !imagick_setfontsize( $handle, 16 ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_setfontsize() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	if ( !imagick_setfontstyle( $handle, IMAGICK_FONTSTYLE_ITALIC ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_setfontstyle() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
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
