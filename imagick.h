/*
  +----------------------------------------------------------------------+
  | PHP Version 4                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2003 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.02 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/2_02.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Michael Montero   (mmontero@mail.communityconnect.com)       |
  |         Christian Stocker (chregu@bitflux.ch)                        |
  +----------------------------------------------------------------------+

  $Id$
*/

#ifndef IMAGICKH
#define IMAGICKH

	#include <magick/api.h>

	#define IMAGICK_VERSION					"0.9.6"
	#define IMAGICK_TURN_OFF_ERRORS_TO_STDERR		1
	#define IMAGICK_FILTER_UNKNOWN				-1 
	#define IMAGICK_DEPCREATED()				php_error( E_WARNING, "%s(): this function is deprecated and will be removed soon", get_active_function_name( TSRMLS_C ) ) ;

	/*****

	   This structure is used for all image processing.  Anyone calling
	   an imagick function in PHP will be given and will pass around a
	   handle.  This handle is used to identify a structure of the type
	   defined below.

	*****/

	typedef struct
	{
		int		id ;		/* used to pass around within
						   PHP code */
		Image* 		image ;		/* ImageMagick Image structure
						   for the presently selected
						   image */
		ImageInfo* 	image_info ;	/* ImageMagick ImageInfo
						   structure describing the
						   current image */
		ExceptionInfo 	exception ;	/* stores any exceptions */
		DrawInfo*	draw_info ;	/* ImageMagick DrawInfo
						   structure to use when
						   allocating a DrawContext */
		DrawContext	draw_context ;	/* ImageMagick DrawContext
						   structure for vector based
						   drawing */
	} imagick_t ;

	/*****

	   Function prototypes

	*****/

	static void _php_imagick_init( void ) ;
	static void _php_imagick_free_handle_list( zend_rsrc_list_entry* rsrc TSRMLS_DC ) ;
	static imagick_t* _php_imagick_get_handle_struct_from_list( zval** handle_id TSRMLS_DC ) ;
	static void _php_imagick_clean_up_handle( imagick_t* ) ;
	static imagick_t* _php_imagick_alloc_handle( void ) ;
	static void _php_imagick_error_handler( const ExceptionType,
				                const char*, const char* ) ;
	static void _php_imagick_clear_errors( imagick_t* ) ;
	static int _php_imagick_is_error( const imagick_t* ) ;
	static imagick_t* _php_imagick_readimage( const char* ) ;
	static int _php_imagick_first_image_in_list( imagick_t* ) ;
	static int _php_imagick_get_geometry_rect( imagick_t*, long, long,
						   char*, RectangleInfo* ) ;

#endif /* IMAGICKH */
