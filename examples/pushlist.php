<?
        $handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

	$image_list = imagick_newimagelist() ;
	if ( !$image_list )
	{
		print "Uh-Oh!  Didn't get new image list.<BR>" ;
		exit ;
	}

	imagick_pushlist( $image_list, $handle ) ;

        $handle = imagick_readimage( getcwd() . "/image.jpg" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

	imagick_pushlist( $image_list, $handle ) ;

	print "Total images in list: " . imagick_getlistsize( $image_list ) ;
?>
