<?
	$handle = imagick_readimage( "./image.jpg" ) ;
	if ( !$handle || imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

		print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
		exit ;
	}

	print imagick_getdpiy( $handle ) ;
?>
