<?
        $handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
        }

	$type = imagick_getimagetype( $handle ) ;
	if ( $type == IMAGICK_TYPE_UNDEFINED )
	{
		print "Undefined!<BR>\n" ;
	}
	else if ( $type == IMAGICK_TYPE_BILEVEL )
	{
		print "Bilevel!<BR>\n" ;
	}
	else if ( $type == IMAGICK_TYPE_GRAYSCALE )
	{
		print "Gray scale!<BR>\n" ;
	}
	else if ( $type == IMAGICK_TYPE_GRAYSCALEMATTE )
	{
		print "Gray scale matte!<BR>\n" ;
	}
	else if ( $type == IMAGICK_TYPE_PALETTE )
	{
		print "Palette!<BR>\n" ;
	}
	else if ( $type == IMAGICK_TYPE_PALETTEMATTE )
	{
		print "Palette matte!<BR>\n" ;
	}
	else if ( $type == IMAGICK_TYPE_TRUECOLOR )
	{
		print "True color!<BR>\n" ;
	}
	else if ( $type == IMAGICK_TYPE_TRUECOLORMATTE )
	{
		print "True color matte!<BR>\n" ;
	}
	else if ( $type == IMAGICK_TYPE_COLORSEPARATION )
	{
		print "Color separation!<BR>\n" ;
	}
	else if ( $type == IMAGICK_TYPE_COLORSEPARATIONMATTE )
	{
		print "Color separation matte!<BR>\n" ;
	}
	else if ( $type == IMAGICK_TYPE_OPTIMIZE )
	{
		print "Optimize!<BR>\n" ;
	}
?>
