<?
        $handle = imagick_readimage( "./image.jpg" ) ;
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

        $handle = imagick_readimage( "./animated.gif" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

	imagick_pushlist( $image_list, $handle ) ;

	$mosaic_handle = imagick_mosaic( $image_list ) ;

        if ( !imagick_writeimage( $mosaic_handle, "./new_image.jpg" ) )
        {
                $reason      = imagick_failedreason( $mosaic_handle ) ;
                $description = imagick_faileddescription( $mosaic_handle ) ;

                print "imagick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

        print "Done!<BR>\n" ;
?>
