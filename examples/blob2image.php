<?
	$fp = fopen( "./image.jpg", "r" ) ;
	if ( !$fp )
	{
		print "Cannot open image.jpg!<BR>\n" ;
		exit ;
	}

	$blob = fread( $fp, filesize( "./image.jpg" ) ) ;
	fclose( $fp ) ;

	$handle = imagick_blob2image( $blob ) ;
	if ( imagick_iserror( $handle ) )
	{
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_blob2image() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
	}

	imagick_scale( $handle, 800, 800 ) ;

        if ( !imagick_writeimage( $handle, "./new_image.jpg" ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

        print "Done!<BR>\n" ;
?>
