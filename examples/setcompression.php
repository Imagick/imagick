<?
        $handle = imagick_readimage( "./image.jpg" ) ;
        if ( imagick_iserror( $handle ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "handle failed!<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

	//
	//  For the second parameter to imagick_setcompression(), use one of
	//  these:
	//
	//	IMAGICK_COMPRESSION_UNDEFINED
	//	IMAGICK_COMPRESSION_NONE
	//	IMAGICK_COMPRESSION_BZIP
	//	IMAGICK_COMPRESSION_FAX
	//	IMAGICK_COMPRESSION_GROUP4
	//	IMAGICK_COMPRESSION_JPEG
	//	IMAGICK_COMPRESSION_LOSSLESSJPEG
	//	IMAGICK_COMPRESSION_LZW
	//	IMAGICK_COMPRESSION_RLE
	//	IMAGICK_COMPRESSION_ZIP
	//

	imagick_setcompression( $handle, IMAGICK_COMPRESSION_LOSSLESSJPEG ) ;

        if ( !imagick_writeimage( $handle, "./new_image.jpg" ) )
        {
                $reason      = imagick_failedreason( $handle ) ;
                $description = imagick_faileddescription( $handle ) ;

                print "imagick_writeimage() failed<BR>\nReason: $reason<BR>\nDescription: $description<BR>\n" ;
                exit ;
        }

        header( "Location: images_jpg.html" ) ;
?>
