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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_smart_str.h"
#include "php_imagick.h"
#include "imagick.h"

ZEND_DECLARE_MODULE_GLOBALS( imagick ) ;

/* True global resources - no need for thread safety here */
static int le_imagick ;
static int le_imagick_handle ;

zend_function_entry imagick_functions[] =
{
	/*****

	   Constitute an image.

	*****/

	PHP_FE( imagick_readimage,	 	NULL )
	PHP_FE( imagick_writeimage,		NULL )
	PHP_FE( imagick_writeimages,		NULL )
	PHP_FE( imagick_clonehandle,		NULL )
	PHP_FE( imagick_image2blob,		NULL )
	PHP_FE( imagick_blob2image,		NULL )
	PHP_FE( imagick_getcanvas,		NULL )

	/*****

	   Error handling.

	*****/

	PHP_FE( imagick_iserror,		NULL )
	PHP_FE( imagick_failedreason, 		NULL )
	PHP_FE( imagick_faileddescription, 	NULL )

	/*****

	   Getting information about an image.

	*****/

	PHP_FE( imagick_getwidth,		NULL )
	PHP_FE( imagick_getheight,		NULL )
	PHP_FE( imagick_getmimetype,		NULL )
	PHP_FE( imagick_getimagedepth,		NULL )
	PHP_FE( imagick_getnumbercolors,	NULL )
	PHP_FE( imagick_isgrayimage,		NULL )
	PHP_FE( imagick_ismonochromeimage,	NULL )
	PHP_FE( imagick_isopaqueimage,		NULL )
	PHP_FE( imagick_ispaletteimage,		NULL )
	PHP_FE( imagick_getimagetype,		NULL )
	PHP_FE( imagick_isimagesequal,		NULL )
	PHP_FE( imagick_getmagick,		NULL )

	/*****

	   Convert an image.

	*****/

	PHP_FE( imagick_convert, 		NULL )

	/*****

	   Resize an image.

	*****/

	PHP_FE( imagick_resize,			NULL )
	PHP_FE( imagick_magnify,		NULL )
	PHP_FE( imagick_minify,			NULL )
	PHP_FE( imagick_scale,			NULL )
	PHP_FE( imagick_sample,			NULL )
	PHP_FE( imagick_zoom,			NULL )

	/*****

	   Font settings.

	*****/

	PHP_FE( imagick_setfontsize,		NULL )
	PHP_FE( imagick_setfontstyle,		NULL )
	PHP_FE( imagick_setfontface,		NULL )

	/*****

	   Draw on an image.

	*****/

	PHP_FE( imagick_begindraw,		NULL )
	PHP_FE( imagick_drawannotation,		NULL )
	PHP_FE( imagick_drawrectangle,		NULL )
	PHP_FE( imagick_drawellipse,		NULL )
	PHP_FE( imagick_drawarc,		NULL )
	PHP_FE( imagick_drawcircle,		NULL )
	PHP_FE( imagick_drawline,		NULL )
	PHP_FE( imagick_drawpoint,		NULL )
	PHP_FE( imagick_setfillcolor,		NULL )

	/*****

	   Enhance an image.

	*****/

	PHP_FE( imagick_rotate,			NULL )
	PHP_FE( imagick_shear,			NULL )
	PHP_FE( imagick_contrast,		NULL )
	PHP_FE( imagick_equalize,		NULL )
	PHP_FE( imagick_gamma,			NULL )
	PHP_FE( imagick_level,			NULL )
	PHP_FE( imagick_modulate,		NULL )
	PHP_FE( imagick_negate,			NULL )
	PHP_FE( imagick_normalize,		NULL )

	/*****

	   Decorate an image.

	*****/

	PHP_FE( imagick_border,			NULL )
	PHP_FE( imagick_frame,			NULL )
	PHP_FE( imagick_raise,			NULL )

	/*****

	   Special effects.

	*****/

	PHP_FE( imagick_charcoal,		NULL )
	PHP_FE( imagick_implode,	 	NULL )
	PHP_FE( imagick_oilpaint,	 	NULL )
	PHP_FE( imagick_solarize,		NULL )
	PHP_FE( imagick_swirl,			NULL )
	PHP_FE( imagick_wave,			NULL )

	/*****

	   Image effects.

	*****/

	PHP_FE( imagick_blur,			NULL )
	PHP_FE( imagick_despeckle,		NULL )
	PHP_FE( imagick_edge,			NULL )
	PHP_FE( imagick_emboss,			NULL )
	PHP_FE( imagick_enhance,		NULL )
	PHP_FE( imagick_gaussianblur,		NULL )
	PHP_FE( imagick_medianfilter,		NULL )
	PHP_FE( imagick_motionblur,		NULL )
	PHP_FE( imagick_reducenoise,		NULL )
	PHP_FE( imagick_shade,			NULL )
	PHP_FE( imagick_sharpen,		NULL )
	PHP_FE( imagick_spread,			NULL )
	PHP_FE( imagick_threshold,		NULL )
	PHP_FE( imagick_unsharpmask,		NULL )
	PHP_FE( imagick_flatten,		NULL )

        /*****

           Image list manipulation.

        *****/

	PHP_FE( imagick_next,			NULL )
	PHP_FE( imagick_prev,			NULL )
	PHP_FE( imagick_first,			NULL )
	PHP_FE( imagick_goto,			NULL )
	PHP_FE( imagick_getlistsize,		NULL )
	PHP_FE( imagick_getlistindex,		NULL )
	PHP_FE( imagick_getimagefromlist,	NULL )

	/*****

	   Transform an image.

	*****/

	PHP_FE( imagick_chop,			NULL )
	PHP_FE( imagick_crop,			NULL )
	PHP_FE( imagick_flip,			NULL )
	PHP_FE( imagick_flop,			NULL )
	PHP_FE( imagick_roll,			NULL )
	PHP_FE( imagick_profile,		NULL )
	PHP_FE( imagick_rgbtransform,		NULL )
	PHP_FE( imagick_transformrgb,		NULL )

	/*****

	   Quantize an image.

	*****/

	PHP_FE( imagick_ordereddither,		NULL )

	/*****

	   Composite an image.

	*****/

	PHP_FE( imagick_composite,		NULL )

	/*****

	   Clean up.

	*****/

	PHP_FE( imagick_destroyhandle,		NULL )

        /*****

           Backward compatibility with old imagick extension.

        *****/

        PHP_FE(	imagick_read,			NULL )
	PHP_FE( imagick_free,			NULL )

        /*****

           Deprecated functions from old imagick extension.

        *****/

        PHP_FE( imagick_add_resource,		NULL )
        PHP_FE( imagick_list_magickinfo,	NULL )
        PHP_FE( imagick_new,			NULL )
        PHP_FE( imagick_init,			NULL )
        PHP_FE( imagick_copy_sample,		NULL )
        PHP_FE( imagick_copy_resize,		NULL )
        PHP_FE( imagick_copy_crop,		NULL )
        PHP_FE( imagick_copy_shear,		NULL )
        PHP_FE( imagick_copy_rotate,		NULL )
        PHP_FE( imagick_copy_morph,		NULL )
	PHP_FE( imagick_dump,			NULL )

	/*****

	   Aliases

	*****/

	PHP_FALIAS( imagick_write, imagick_writeimage, NULL )

	{NULL, NULL, NULL}
} ;

zend_module_entry imagick_module_entry =
{
	STANDARD_MODULE_HEADER,
	"imagick",
	imagick_functions,
	PHP_MINIT( imagick ),	
	NULL,
	PHP_RINIT( imagick ),
	PHP_RSHUTDOWN( imagick ),
	PHP_MINFO( imagick ),
	IMAGICK_VERSION,
	STANDARD_MODULE_PROPERTIES
} ;

#ifdef COMPILE_DL_IMAGICK
	ZEND_GET_MODULE( imagick )
#endif

static void php_imagick_init_globals (zend_imagick_globals *g) 
{ 
	g->imagick_was_init = 0; 
}

PHP_MINIT_FUNCTION( imagick )
{
	ZEND_INIT_MODULE_GLOBALS(imagick, php_imagick_init_globals, NULL);
	le_imagick_handle = zend_register_list_destructors_ex(
				_php_imagick_free_handle_list, NULL,
				"imagick handle", module_number ) ;

	/*****

	   Register the filter types.  These values are taken from
	   magick/classify.h in the ImageMagick API.  The enumeration is
	   called FilterTypes.

	*****/

	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_UNDEFINED", UndefinedFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_POINT", PointFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_BOX", BoxFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_TRIANGLE", TriangleFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_HERMITE", HermiteFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_HANNING", HanningFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_HAMMING", HammingFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_BLACKMAN", BlackmanFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_GAUSSIAN", GaussianFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_QUADRATIC", QuadraticFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_CUBIC", CubicFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_CATROM", CatromFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_MITCHELL", MitchellFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_LANCZOS", LanczosFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_BESSEL", BesselFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_SINC", SincFilter,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FILTER_UNKNOWN",
				IMAGICK_FILTER_UNKNOWN,
				CONST_CS | CONST_PERSISTENT ) ;

	/*****

	   Register the font styles.  These values are taken from
	   magick/classify.h in the ImageMagick API.  The enumeration is
	   called StyleType.

	*****/

	REGISTER_LONG_CONSTANT( "IMAGICK_FONTSTYLE_NORMAL", NormalStyle,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FONTSTYLE_ITALIC", ItalicStyle,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FONTSTYLE_OBLIQUE", ObliqueStyle,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_FONTSTYLE_ANY", AnyStyle,
				CONST_CS | CONST_PERSISTENT ) ;

	/*****

	   Register constants for raising/lowering an image when calling
	   imagick_raise().

	*****/

	REGISTER_LONG_CONSTANT( "IMAGICK_RAISE", 1,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_LOWER", 0,
				CONST_CS | CONST_PERSISTENT ) ;

	/*****

	   Register constants for composite types.

	*****/

	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_UNDEFINED",
				UndefinedCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_OVER",
				OverCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_IN",
				InCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_OUT",
				OutCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_ATOP",
				AtopCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_XOR",
				XorCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_PLUS",
				PlusCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_MINUS",
				MinusCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_ADD",
				AddCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_SUBTRACT",
				SubtractCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_DIFFERENCE",
				DifferenceCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_MULTIPLY",
				MultiplyCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_BUMPMAP",
				BumpmapCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_COPY",
				CopyCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_COPYRED",
				CopyRedCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_COPYGREEN",
				CopyGreenCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_COPYBLUE",
				CopyBlueCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_COPYOPACITY",
				CopyOpacityCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_CLEAR",
				ClearCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_DISSOLVE",
				DissolveCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_DISPLACE",
				DisplaceCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_MODULATE",
				ModulateCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_THRESHOLD",
				ThresholdCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_NONE",
				NoCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_DARKEN",
				DarkenCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_LIGHTEN",
				LightenCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_HUE",
				HueCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_SATURATE",
				SaturateCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_COLORIZE",
				ColorizeCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_LUMINIZE",
				LuminizeCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_SCREEN",
				ScreenCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COMPOSITE_OP_OVERLAY",
				OverlayCompositeOp,
				CONST_CS | CONST_PERSISTENT ) ;

	/*****

	   Register constants for manipulating an image's profile.

	*****/

	REGISTER_LONG_CONSTANT( "IMAGICK_PROFILE_OWN", 0,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_PROFILE_COPY", 1,
				CONST_CS | CONST_PERSISTENT ) ;

	/*****

	   Register constants for image types.

	*****/

	REGISTER_LONG_CONSTANT( "IMAGICK_TYPE_UNDEFINED",
				UndefinedType,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_TYPE_BILEVEL",
				BilevelType,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_TYPE_GRAYSCALE",
				GrayscaleType,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_TYPE_GRAYSCALEMATTE",
				GrayscaleMatteType,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_TYPE_PALETTE",
				PaletteType,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_TYPE_PALETTEMATTE",
				PaletteMatteType,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_TYPE_TRUECOLOR",
				TrueColorType,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_TYPE_TRUECOLORMATTE",
				TrueColorMatteType,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_TYPE_COLORSEPARATION",
				ColorSeparationType,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_TYPE_COLORSEPARATIONMATTE",
				ColorSeparationMatteType,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_TYPE_OPTIMIZE",
				OptimizeType,
				CONST_CS | CONST_PERSISTENT ) ;

	/*****

	   Register constants for colorspace types.

	*****/

	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_UNDEFINED",
				UndefinedColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_RGB",
				RGBColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_GRAY",
				GRAYColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_TRANSPARENT",
				TransparentColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_OHTA",
				OHTAColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_XYZ",
				XYZColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_YCBCR",
				YCbCrColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_YCC",
				YCCColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_YIQ",
				YIQColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_YPBPR",
				YPbPrColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_YUV",
				YUVColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_CMYK",
				CMYKColorspace,
				CONST_CS | CONST_PERSISTENT ) ;
	REGISTER_LONG_CONSTANT( "IMAGICK_COLORSPACE_SRGB",
				sRGBColorspace,
				CONST_CS | CONST_PERSISTENT ) ;

	return SUCCESS ;
}

PHP_RINIT_FUNCTION( imagick )
{
	IMAGICK_G( imagick_was_init ) = 0 ;
	
	return SUCCESS ;
}

PHP_RSHUTDOWN_FUNCTION( imagick )
{
	if ( IMAGICK_G( imagick_was_init ) == 1 )
	{
		DestroyMagick() ;
	}

	return SUCCESS ;
}

PHP_MINFO_FUNCTION( imagick )
{
	const MagickInfo*          magick_info ;
	const register MagickInfo* p ;
	const TypeInfo*            type_info ;
	ExceptionInfo              exception ;
	char                       buffer[ 512 ] ;
	smart_str		   format_list = { 0 } ;

	php_info_print_table_start() ;
	php_info_print_table_header( 2, "ImageMagick support", "enabled" ) ;
	php_info_print_table_row( 2, "ImageMagick version",
				  MagickLibVersionText ) ;
	php_info_print_table_row( 2, "PHP imagick version", IMAGICK_VERSION ) ; 

	snprintf( buffer, 512, "%ld\0", MaxRGB ) ;
	php_info_print_table_row( 2, "MaxRGB", buffer ) ;

	/*****

	   Get the list of available image formats.

	*****/

	magick_info = GetMagickInfo( NULL, &exception ) ;
	if ( magick_info != ( MagickInfo* )NULL )
	{
		for ( p = magick_info ; p != ( MagickInfo* )NULL ; p = p->next )
		{
			smart_str_appendl( &format_list, p->name,
					   strlen( p->name ) ) ;
			if ( p->next )
			{
				smart_str_appendl( &format_list, ", ", 2 ) ;
			}
		}

		smart_str_0( &format_list ) ;
		php_info_print_table_row( 2, "Supported image formats",
					  format_list.c ) ;
	}
	else
	{
		php_error( E_WARNING, "%s(): magick_info struct is NULL after call to GetMagickInfo()", get_active_function_name( TSRMLS_C ) ) ;
	}

	/*****

	   Get a list of the available font names.

	*****/

	type_info = GetTypeInfo( "*", &exception ) ;
	if ( type_info != ( TypeInfo* )NULL )
	{
		while( type_info )
		{
			snprintf( buffer, 512, "%s - %s\0", type_info->family,
				  type_info->name ) ;
			php_info_print_table_row( 2, "Font Family - Name",
						  buffer ) ;

			type_info = type_info->next ;
		}
	}
	else
	{
		php_info_print_table_row(2, "Fonts", "No fonts found");
	}

	php_info_print_table_end();
}

/******************************************************************************
 *
 *  Constitute an image.
 *
 */
 
PHP_FUNCTION( imagick_readimage )
{
	char*      file_name ;		/* the file name to use to retrieve
					   the image */
	int        file_name_len ;	/* the length of the file name */
	imagick_t* handle ;		/* the handle for this image */ 

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&file_name, &file_name_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_readimage( file_name ) ;
	RETURN_RESOURCE( handle->id ) ;
}

PHP_FUNCTION( imagick_writeimage )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	char*      file_name ;		/* the location to which to write out
					   the image */
	int        file_name_len ;	/* the length of the file name */	
	imagick_t* handle ;		/* the handle for this image */ 

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rs",
                        &handle_id, &file_name, &file_name_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->image_info )
	{
		php_error( E_WARNING, "%s(): image_info is empty ",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	if ( !handle->image )
	{
		php_error( E_WARNING, "%s(): image is empty",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	if ( file_name_len >= MaxTextExtent )
	{
		php_error( E_WARNING, "%s(): file name is too long",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	strncpy( handle->image->filename, file_name, MaxTextExtent - 1 ) ;

	if ( !WriteImage( handle->image_info, handle->image ) )
	{
		ThrowException( &handle->exception, FileOpenError,
			"could not write to file in _php_imagick_writeimage()",
			file_name ) ;
		RETURN_FALSE ;
	}

	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_writeimages )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	char*      file_name ;		/* the location to which to write out
					   the image */
	int	   file_name_len ;	/* the length of the file name */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rs",
			&handle_id, &file_name, &file_name_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->image_info )
	{
		php_error( E_WARNING, "%s(): image_info is empty ",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	if ( !handle->image )
	{
		php_error( E_WARNING, "%s(): image is empty",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	if ( file_name_len >= MaxTextExtent )
	{
		php_error( E_WARNING, "%s(); file name is too long",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	if ( !WriteImages( handle->image_info, handle->image, file_name,
			   &handle->exception ) )
	{
		ThrowException( &handle->exception, FileOpenError,
			"could not write to file in imagick_writeimages()",
			file_name ) ;
		RETURN_FALSE ;
	}

	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_clonehandle )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	imagick_t* new_handle ;		/* the new, cloned handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	new_handle = _php_imagick_alloc_handle() ;
	if ( !new_handle )
	{
		RETURN_FALSE ;
	}

	if ( handle->image )
	{
		if ( GetImageListSize( handle->image ) <= 1 )
		{
			/*****

			   This is a single image, not a list.

			*****/

			new_handle->image = CloneImage( handle->image, 0, 0, 1,
			                                &handle->exception ) ;
		}
		else
		{
			new_handle->image = CloneImageList( handle->image,
		    				&handle->exception ) ;
		}

		if ( _php_imagick_is_error( handle ) )
		{
			_php_imagick_clean_up_handle( new_handle ) ;
			RETURN_FALSE ;
		}
	}

	/*****

	   _php_imagick_alloc_handle() already created a NULL image_info
	   structure.  We want to safely clean that up before assigning the
	   new one.

	*****/

	if ( new_handle->image_info )
	{
		DestroyImageInfo( new_handle->image_info ) ;
	}
	new_handle->image_info = CloneImageInfo( handle->image_info ) ;

	new_handle->id = zend_list_insert( new_handle, le_imagick_handle ) ;
	RETURN_RESOURCE( new_handle->id ) ;
}

PHP_FUNCTION( imagick_image2blob )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	void*      blob_data ;		/* the image BLOB data */
	size_t     blob_size ;		/* the size of the image BLOB */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->image )
	{
		ThrowException( &handle->exception, FatalErrorException,
			"image is empty in imagick_image2blob()", NULL ) ;
		RETURN_FALSE ;
	}

	blob_data = ImageToBlob( handle->image_info, handle->image,
				 &blob_size, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( blob_data )
		{
			/*****

			   ImageMagick allocated this memory using malloc(),
			   so we'll free it just to make sure.

			*****/

			free( blob_data ) ;
		}

		RETURN_FALSE ;
	}

	if ( !blob_data )
	{
		ThrowException( &handle->exception, FatalErrorException, "blob_data is empty in imagick_image2blob() after call to ImageToBlob()", NULL ) ;
		RETURN_FALSE ;
	}

	RETURN_STRINGL( ( char* )blob_data, blob_size, 1 ) ;
}

PHP_FUNCTION( imagick_blob2image )
{
	char*      blob_data ;		/* the blob data to use to create the
					   image */
	long       blob_data_len ;	/* string length of blob_data */
	imagick_t* handle ;		/* the handle for this image */ 

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&blob_data, &blob_data_len ) == FAILURE )
	{
		return ;
	}

	if ( IMAGICK_G( imagick_was_init ) == 0 )
	{
		/*****

		   Initialize the ImageMagick engine.

		*****/

		_php_imagick_init() ;
	}

	handle = _php_imagick_alloc_handle() ;
	if ( !handle )
	{
		RETURN_FALSE ;
	}
	handle->id = zend_list_insert( handle, le_imagick_handle ) ;

	handle->image = BlobToImage( handle->image_info, ( void* )blob_data,
				     blob_data_len, &handle->exception ) ;
	RETURN_RESOURCE( handle->id ) ;
}

PHP_FUNCTION( imagick_getcanvas )
{
	char* 	   color ;			/* the color of the canvas */
	int	   color_len ;			/* the length of the color */
	long       width ;			/* the width of the canvas */
	long       height ;			/* the height of the canvas */	
	imagick_t* handle ;			/* the handle for this image */ 
	char       buffer[ MaxTextExtent ] ;	/* buffer for creating
						   strings */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "sll",
			&color, &color_len, &width, &height ) == FAILURE )
	{
		return ;
	}

	if ( IMAGICK_G( imagick_was_init ) == 0 )
	{
		/*****

		   Initialize the ImageMagick engine.

		*****/

		_php_imagick_init() ;
	}

	handle = _php_imagick_alloc_handle() ;
	if ( !handle )
	{
		RETURN_FALSE ;
	}
	handle->id = zend_list_insert( handle, le_imagick_handle ) ;

	FormatString( buffer, "%ldx%ld", width, height ) ;
	CloneString( &handle->image_info->size, buffer ) ;

	FormatString( buffer, "xc:%s\0", color ) ;
	strncpy( handle->image_info->filename, buffer, strlen( buffer ) ) ;

	handle->image = ReadImage( handle->image_info, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_RESOURCE( handle->id ) ;
	}

	if ( !handle->image )
	{
		ThrowException( &handle->exception, FatalErrorException,
			"after ReadImage(), image is empty imagick_getcanvas()",
			NULL ) ;
		RETURN_RESOURCE( handle->id ) ;
	}

	RETURN_RESOURCE( handle->id ) ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Error handling.
 *
 */

PHP_FUNCTION( imagick_iserror )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_TRUE ;
	}

	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_failedreason )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	if ( handle->exception.reason )
	{
		RETURN_STRINGL( handle->exception.reason,
		                strlen( handle->exception.reason ), 1 ) ;
	}

	if ( handle->image->exception.reason )
	{
		RETURN_STRINGL( handle->image->exception.reason,
		                strlen( handle->image->exception.reason ),
				1 ) ;
	}

	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_faileddescription )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	if ( handle->exception.description )
	{
		RETURN_STRINGL( handle->exception.description,
		                strlen( handle->exception.description ), 1 ) ;
	}

	if ( handle->image->exception.description )
	{
		RETURN_STRINGL( handle->image->exception.description,
		                strlen( handle->image->exception.description ),
			        1 ) ;
	}

	RETURN_FALSE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Getting information about an image.
 *
 */

PHP_FUNCTION( imagick_getwidth )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	RETURN_LONG( handle->image->columns ) ;
}

PHP_FUNCTION( imagick_getheight )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	RETURN_LONG( handle->image->rows ) ;
}

PHP_FUNCTION( imagick_getmimetype )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	char*      mime_type ;		/* the image MIME type */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	mime_type = MagickToMime( handle->image->magick ) ;
	RETURN_STRINGL( mime_type, strlen( mime_type ), 1 ) ;
}

PHP_FUNCTION( imagick_getimagedepth )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	RETURN_LONG( GetImageDepth( handle->image, &handle->exception ) ) ;
}

PHP_FUNCTION( imagick_getnumbercolors )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	char*      hist_file ;		/* the path to the histogram file
					   to write; note this can be
					   /dev/null */
	int        hist_file_len ;	/* string length of hist_file */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	FILE*      fp ;			/* file pointer to the histogram
					   file */
	long       unique_colors ;	/* the number of unique colors */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rs",
			&handle_id, &hist_file, &hist_file_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( hist_file_len <= 0 )
	{
		ThrowException( &handle->exception, ErrorException, "you must specify a histogram file; note the histogram file can be /dev/null", NULL ) ;
		RETURN_FALSE ;
	}

	fp = fopen( hist_file, "w" ) ;
	if ( !fp )
	{
		ThrowException( &handle->exception, FatalErrorException, "could not write to histogram file", NULL ) ;
		RETURN_FALSE ;
	}

	unique_colors = GetNumberColors( handle->image, fp,
					 &handle->exception ) ;
	fclose( fp ) ;

	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_LONG( unique_colors ) ;
}

PHP_FUNCTION( imagick_isgrayimage )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !IsGrayImage( handle->image, &handle->exception ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_ismonochromeimage )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !IsMonochromeImage( handle->image, &handle->exception ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_isopaqueimage )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !IsOpaqueImage( handle->image, &handle->exception ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_ispaletteimage )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !IsPaletteImage( handle->image, &handle->exception ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_getimagetype )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	ImageType  type ;		/* the image type, to be returned to
					   PHP */

	type = UndefinedType ;

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	type = GetImageType( handle->image, &handle->exception ) ;

	RETURN_LONG( type ) ;
}

PHP_FUNCTION( imagick_isimagesequal )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	zval*	   ref_id ;		/* the handle identifier coming from
					   the PHP environment for the
					   reference image */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	imagick_t* ref_handle ;		/* the actual imagick_t struct for the
					   handle of the reference image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rr",
			&handle_id, &ref_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	ref_handle = _php_imagick_get_handle_struct_from_list( &ref_id TSRMLS_CC ) ;
	if ( !ref_handle )
	{
		php_error( E_WARNING, "%s(): reference handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;
	_php_imagick_clear_errors( ref_handle ) ;

	if ( handle->image     == ( Image* )NULL ||
	     ref_handle->image == ( Image* )NULL )
	{
		RETURN_FALSE ;
	}

	if ( !IsImagesEqual( handle->image, ref_handle->image ) )
	{
		RETURN_FALSE ;
	}

	if ( _php_imagick_is_error( handle ) ||
	     _php_imagick_is_error( ref_handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_getmagick )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	RETURN_STRINGL( handle->image->magick, strlen( handle->image->magick ),
			1 ) ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Convert an image.
 *
 */

PHP_FUNCTION( imagick_convert )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	char*	   type ;		/* the image type to convert to -
					   execute "convert -list format" and
					   use the rightmost column for this
				  	   value */
	int        type_len ;		/* length of the type string */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rs",
			&handle_id, &type, &type_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->image )
	{
		php_error( E_WARNING, "%s(): image is empty",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	if ( type_len >= MaxTextExtent )
	{
		php_error( E_WARNING, "%s(): type is too long",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	strncpy( handle->image->magick, type, MaxTextExtent - 1 ) ;

	/*****

	   Note, when the image is written or transferred to a BLOB it will
	   be rendered as the type specified.

	*****/

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Resize an image.
 *
 */

PHP_FUNCTION( imagick_resize )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	long	      cols ;		/* the number of columns to resize to */
	long	      rows ;		/* the number of rows to resize to */
	long	      filter ;		/* the filter type to use when re-
					   sizing - one of IMAGICK_FILTER_* */
	double	      blur ;		/* the factor where > 1 is blurry,
					   < 1 is sharp */
	char*         geo_mods ;	/* modifiers to the geometry to create
					   an ImageMagick geometry string */
	int           geo_mods_len ;	/* string length of geo_mods */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*        new_image ;	/* the new, resized image */
	RectangleInfo new_geometry ;	/* the new image geometry with the
					   geometry string applied */

	geo_mods     = NULL ;
	geo_mods_len = 0 ;

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rllld|s",
			&handle_id, &cols, &rows, &filter, &blur,
			&geo_mods, &geo_mods_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	/*****

	   If the user specifies they don't know what filter to use, then
	   take the filter that is already defined by the image.

	*****/

	if ( filter == IMAGICK_FILTER_UNKNOWN )
	{
		filter = handle->image->filter ;
	}

	if ( !_php_imagick_get_geometry_rect( handle, cols, rows, geo_mods,
			&new_geometry ) )
	{
		RETURN_FALSE ;
	}

	new_image = ResizeImage( handle->image, new_geometry.width,
				 new_geometry.height, ( FilterTypes )filter,
				 blur, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_magnify )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*     new_image ;		/* the new, magnified image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

        _php_imagick_clear_errors( handle ) ;

	new_image = MagnifyImage( handle->image, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_minify )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*     new_image ;		/* the new, minified image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	new_image = MinifyImage( handle->image, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_scale )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	long          cols ;		/* the number of columns to scale to */
	long 	      rows ;		/* the number of rows to scale to */
	char*         geo_mods ;	/* modifiers to the geometry to create
					   an ImageMagick geometry string */
	int           geo_mods_len ;	/* string length of geo_mods */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*        new_image ;	/* the new, minified image */
	RectangleInfo new_geometry ;	/* the new image geometry with the
					   geometry string applied */

	geo_mods     = NULL ;
	geo_mods_len = 0 ;

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rll|s",
			&handle_id, &cols, &rows, &geo_mods,
			&geo_mods_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !_php_imagick_get_geometry_rect( handle, cols, rows, geo_mods,
			&new_geometry ) )
	{
		RETURN_FALSE ;
	}

	new_image = ScaleImage( handle->image, new_geometry.width,
				new_geometry.height, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_sample )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	long	      cols ;		/* the number of columns to scale to */
	long	      rows ;		/* the number of rows to scale to */
	char*         geo_mods ;	/* modifiers to the geometry to create
					   an ImageMagick geometry string */
	int           geo_mods_len ;	/* string length of geo_mods */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*        new_image ;	/* the new, minified image */
	RectangleInfo new_geometry ;	/* the new image geometry with the
					   geometry string applied */

	geo_mods     = NULL ;
	geo_mods_len = 0 ;

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rll|s",
			&handle_id, &cols, &rows, &geo_mods,
			&geo_mods_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !_php_imagick_get_geometry_rect( handle, cols, rows, geo_mods,
			&new_geometry ) )
	{
		RETURN_FALSE ;
	}

	new_image = SampleImage( handle->image, new_geometry.width,
				 new_geometry.height, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_zoom )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	long	      cols ;		/* the number of columns to scale to */
	long	      rows ;		/* the number of rows to scale to */
	char*         geo_mods ;	/* modifiers to the geometry to create
					   an ImageMagick geometry string */
	int           geo_mods_len ;	/* string length of geo_mods */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*        new_image ;	/* the new, minified image */
	RectangleInfo new_geometry ;	/* the new image geometry with the
					   geometry string applied */

	geo_mods     = NULL ;
	geo_mods_len = 0 ;

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rll|s",
			&handle_id, &cols, &rows, &geo_mods,
			&geo_mods_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !_php_imagick_get_geometry_rect( handle, cols, rows, geo_mods,
			&new_geometry ) )
	{
		RETURN_FALSE ;
	}

	new_image = ZoomImage( handle->image, new_geometry.width,
			       new_geometry.height, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Font settings.
 *
 */

PHP_FUNCTION( imagick_setfontsize )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	double     size ;		/* pointsize for font */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rd",
			&handle_id, &size ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	if ( !handle->draw_info || !handle->draw_context )
	{
		ThrowException( &handle->exception, WarningException,
			"you must call imagick_begindraw() first", NULL ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	DrawSetFontSize( handle->draw_context, size ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_setfontstyle )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	long	   style ;		/* the font style - one of
					   IMAGICK_FONTSTYLE_* */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rl",
			&handle_id, &style ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->draw_info || !handle->draw_context )
	{
		ThrowException( &handle->exception, WarningException,
			"you must call imagick_begindraw() first", NULL ) ;
		RETURN_FALSE ;
	}

	DrawSetFontStyle( handle->draw_context, ( StyleType )style ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_setfontface )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	char*	   face ;		/* the font face to use; see phpinfo()
					   for available fonts */
	int	   face_len ;		/* string length of face */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rs",
			&handle_id, &face, &face_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->draw_info || !handle->draw_context )
	{
		ThrowException( &handle->exception, WarningException,
			"you must call imagick_begindraw() first", NULL ) ;
		RETURN_FALSE ;
	}

	DrawSetFont( handle->draw_context, face ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Draw on an image.
 *
 */

PHP_FUNCTION( imagick_begindraw )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( handle->image_info && !handle->draw_info )
	{
		handle->draw_info = CloneDrawInfo( handle->image_info,
					           ( DrawInfo* )NULL ) ;
		if ( !handle->draw_info )
		{
			php_error( E_ERROR, "%s(): CloneDrawInfo did not properly allocate DrawInfo structure", get_active_function_name( TSRMLS_C ) ) ;
			RETURN_FALSE ;
		}
	}
	else
	{
		php_error( E_WARNING, "%s(): attempt to create draw_info with CloneDrawInfo() and empty image_info", get_active_function_name( TSRMLS_C ) ) ;
	}

	if ( handle->draw_info && handle->image && !handle->draw_context )
	{
		handle->draw_context = DrawAllocateContext( handle->draw_info,
						            handle->image ) ;
		if ( !handle->draw_context )
		{
			php_error( E_ERROR, "%s(): DrawAllocateContext() did not properly allocate DrawContext structure", get_active_function_name( TSRMLS_C ) ) ;
			RETURN_FALSE ;
		}
	}
	else
	{
		php_error( E_WARNING, "%s(): attempt to create draw_context with DrawAllocateContext() and empty draw_info or image", get_active_function_name( TSRMLS_C ) ) ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_drawannotation )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	double	   coord_x ;		/* X coordinate to begin drawing text */
	double	   coord_y ;		/* Y coordinate to begin drawing text */
	char*	   text ;		/* the text to draw */
	int 	   text_len ;		/* string length of text */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdds",
			&handle_id, &coord_x, &coord_y, &text, &text_len )
				== FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->draw_info || !handle->draw_context )
	{
		ThrowException( &handle->exception, WarningException,
			"you must call imagick_begindraw() first", NULL ) ;
		RETURN_FALSE ;
	}

	DrawAnnotation( handle->draw_context, coord_x, coord_y, text ) ;

	DrawRender( handle->draw_context ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_drawrectangle )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	double	   coord_x1 ;		/* x of upper left */
	double	   coord_y1 ;		/* y of upper left */
	double	   coord_x2 ;		/* x of lower right */
	double	   coord_y2 ;		/* y of lower right */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdddd",
			&handle_id, &coord_x1, &coord_y1, &coord_x2,
			&coord_y2 ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->draw_info || !handle->draw_context )
	{
		ThrowException( &handle->exception, WarningException,
			"you must call imagick_begindraw() first", NULL ) ;
		RETURN_FALSE ;
	}

	DrawRectangle( handle->draw_context, coord_x1, coord_y1, coord_x2,
		       coord_y2 ) ;

	DrawRender( handle->draw_context ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_drawellipse )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	double	   coord_x ;		/* origin x coordinate */
	double	   coord_y ;		/* origin y coordinate */
	double	   radius_x ;		/* x radius */
	double	   radius_y ;		/* y radius */
	double	   start_rot ;		/* start rotation in degrees */
	double	   end_rot ;		/* end rotation in degrees */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdddddd",
			&handle_id, &coord_x, &coord_y, &radius_x, &radius_y,
			&start_rot, &end_rot ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->draw_info || !handle->draw_context )
	{
		ThrowException( &handle->exception, WarningException,
			"you must call imagick_begindraw() first", NULL ) ;
		RETURN_FALSE ;
	}

	DrawEllipse( handle->draw_context, coord_x, coord_y, radius_x,
		     radius_y, start_rot, end_rot ) ;

	DrawRender( handle->draw_context ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_drawarc )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	double	   coord_x1 ;		/* starting x coordinate */
	double	   coord_y1 ;		/* starting y coordinate */
	double	   coord_x2 ;		/* ending x coordinate */
	double	   coord_y2 ;		/* ending y coordinate */
	double	   start_rot ;		/* start rotation in degrees */
	double	   end_rot ;		/* end rotation in degrees */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdddddd",
			&handle_id, &coord_x1, &coord_y1, &coord_x2, &coord_y2,
			&start_rot, &end_rot ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->draw_info || !handle->draw_context )
	{
		ThrowException( &handle->exception, WarningException,
			"you must call imagick_begindraw() first", NULL ) ;
		RETURN_FALSE ;
	}

	DrawArc( handle->draw_context, coord_x1, coord_y1, coord_x2,
		 coord_y2, start_rot, end_rot ) ;

	DrawRender( handle->draw_context ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_drawcircle )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	double	   coord_x1 ;		/* origin x coordinate */
	double	   coord_y1 ;		/* origin y coordinate */
	double	   coord_x2 ;		/* perimater x coordinate */
	double	   coord_y2 ;		/* perimater y coordinate */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdddd",
			&handle_id, &coord_x1, &coord_y1, &coord_x2,
			&coord_y2 ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->draw_info || !handle->draw_context )
	{
		ThrowException( &handle->exception, WarningException,
			"you must call imagick_begindraw() first", NULL ) ;
		RETURN_FALSE ;
	}

	DrawCircle( handle->draw_context, coord_x1, coord_y1, coord_x2,
		    coord_y2 ) ;

	DrawRender( handle->draw_context ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_drawline )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	double	   coord_x1 ;		/* starting x coordinate */
	double	   coord_y1 ;		/* starting y coordinate */
	double	   coord_x2 ;		/* ending x coordinate */
	double	   coord_y2 ;		/* ending y coordinate */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdddd",
			&handle_id, &coord_x1, &coord_y1, &coord_x2,
			&coord_y2 ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->draw_info || !handle->draw_context )
	{
		ThrowException( &handle->exception, WarningException,
			"you must call imagick_begindraw() first", NULL ) ;
		RETURN_FALSE ;
	}

	DrawLine( handle->draw_context, coord_x1, coord_y1, coord_x2,
		  coord_y2 ) ;

	DrawRender( handle->draw_context ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_drawpoint )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	double	   coord_x ;		/* x coordinate */
	double	   coord_y ;		/* y coordinate */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdd",
			&handle_id, &coord_x, &coord_y ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->draw_info || !handle->draw_context )
	{
		ThrowException( &handle->exception, WarningException,
			"you must call imagick_begindraw() first", NULL ) ;
		RETURN_FALSE ;
	}

	DrawPoint( handle->draw_context, coord_x, coord_y ) ;

	DrawRender( handle->draw_context ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_setfillcolor )
{
	zval* 	    handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	char*       color ;		/* the color in the format #ff00ff */
	int	    color_len ;		/* string length of color */
	imagick_t*  handle ;		/* the actual imagick_t struct for the
					   handle */
	PixelPacket pixel_packet ;	/* the struct containing the color
				  	   information */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rs",
			&handle_id, &color, &color_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	if ( !handle->draw_info || !handle->draw_context )
	{
		ThrowException( &handle->exception, WarningException,
			"you must call imagick_begindraw() first", NULL ) ;
		RETURN_FALSE ;
	}

	if ( *color != '#' )
	{
		ThrowException( &handle->exception, WarningException,
			"color must be in the format \"#ffff00\"", NULL ) ;
		RETURN_FALSE ;
	}

	QueryColorDatabase( color, &pixel_packet, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	DrawSetFillColor( handle->draw_context, &pixel_packet ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Enhance an image.
 *
 */

PHP_FUNCTION( imagick_rotate )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	double	   degrees ;		/* the number of degrees to rotate;
					   positive angles rotate image
					   counter-clockwise, negative angles
					   rotate clockwise */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*     new_image ;		/* the new, rotated image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rd",
			&handle_id, &degrees ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}
	
	_php_imagick_clear_errors( handle ) ;

	if ( degrees == 0 )
	{
		RETURN_TRUE ;
	}

	new_image = RotateImage( handle->image, degrees, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_shear )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	double	   coord_x ;		/* x coordinate */
	double	   coord_y ;		/* y coordinate */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*     new_image ;		/* the new, rotated image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdd",
			&handle_id, &coord_x, &coord_y ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	new_image = ShearImage( handle->image, coord_x, coord_y,
				&handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_contrast )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	long	   sharpen ;		/* increase or decrease image
					   contrast */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rl",
			&handle_id, &sharpen ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	ContrastImage( handle->image, sharpen ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_equalize )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	EqualizeImage( handle->image ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                RETURN_FALSE ;
        }

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_gamma )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	char*	   level ;		/* define the level of gamma
					   correction; values typically
				           range from 0.8 to 2.3 */
	int	   level_len ;		/* string length of level */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rs",
			&handle_id, &level, &level_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	GammaImage( handle->image, level ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                RETURN_FALSE ;
        }

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_level )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	char*	   level ;		/* define the level of gamma
					   correction; values typically
				           range from 0.8 to 2.3 */
	int	   level_len ;		/* string length of level */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rs",
			&handle_id, &level, &level_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	LevelImage( handle->image, level ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                RETURN_FALSE ;
        }

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_modulate )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	char*	   modulate ;		/* change in brightness, saturation
					   and hue */
	int        modulate_len ;	/* string length of modulate */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rs",
			&handle_id, &modulate, &modulate_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	ModulateImage( handle->image, modulate ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                RETURN_FALSE ;
        }

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_negate )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	long	   gray_scale ;		/* whether only the gray scale within
					   the image should be negated 
					   and hue */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rl",
			&handle_id, &gray_scale ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	NegateImage( handle->image, gray_scale ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                RETURN_FALSE ;
        }

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_normalize )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	NormalizeImage( handle->image ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                RETURN_FALSE ;
        }

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Decorate an image.
 *
 */

PHP_FUNCTION( imagick_border )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	long	      coord_x ;		/* x starting position of border */
	long	      coord_y ;		/* y starting position of border */
	long	      width ;		/* width of border */
	long	      height ;		/* height of border */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */
	RectangleInfo border_info ;	/* struct containing x, y, width and
					   height of border */
	Image*        new_image ;	/* the new image with the border */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rllll",
			&handle_id, &coord_x, &coord_y, &width, &height )
				== FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	border_info.width  = width ;
	border_info.height = height  ;
	border_info.x      = coord_x ;
	border_info.y      = coord_y ;

	new_image = BorderImage( handle->image, &border_info,
				 &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_frame )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	long	   coord_x ;		/* x starting position of border */
	long	   coord_y ;		/* y starting position of border */
	long	   width ;		/* width of border */
	long	   height ;		/* height of border */
	long	   inner_bevel ;	/* the inner bevel */
	long	   outer_bevel ;	/* the outer bevel */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	FrameInfo  frame_info ;		/* struct containing x, y, width,
					   height and bevel values of border */
	Image*     new_image ;		/* the new image with the frame */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rllllll",
			&handle_id, &coord_x, &coord_y, &width, &height,
			&inner_bevel, &outer_bevel ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	frame_info.width       = width ;
	frame_info.height      = height  ;
	frame_info.x           = coord_x ;
	frame_info.y           = coord_y ;
	frame_info.inner_bevel = inner_bevel ;
	frame_info.outer_bevel = outer_bevel ;

	new_image = FrameImage( handle->image, &frame_info,
				&handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_raise )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	long	      coord_x ;		/* x starting position of border */
	long	      coord_y ;		/* y starting position of border */
	long	      width ;		/* width of border */
	long	      height ;		/* height of border */
	long	      raise ;		/* non-zero and the image has a 3-D
					   raise effect, otherwise it looks
					   lowered */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */
	RectangleInfo raise_info ;	/* struct containing x, y, width and
					   height of the raise */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rlllll",
			&handle_id, &coord_x, &coord_y, &width, &height,
			&raise ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	raise_info.width  = width ;
	raise_info.height = height  ;
	raise_info.x      = coord_x ;
	raise_info.y      = coord_y ;

	RaiseImage( handle->image, &raise_info, raise ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Special effects.
 *
 */

PHP_FUNCTION( imagick_charcoal )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	double     radius ;		/* the radius of pixel neighborhood */
	double     sigma ;		/* the standard deviation of the
					   Gaussian, in pixels */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*     new_image ;		/* the new, charcoaled image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdd",
			&handle_id, &radius, &sigma ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	new_image = CharcoalImage( handle->image, radius, sigma,
				   &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_implode )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        double     amount ;             /* define the extent of the implosion */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, imploded image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rd",
			&handle_id, &amount ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = ImplodeImage( handle->image, amount, &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

                RETURN_FALSE ;
        }

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

        RETURN_TRUE ;
}

PHP_FUNCTION( imagick_oilpaint )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        double     radius ;             /* the radius of the circular
					   neighborhood */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, oil painted image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rd",
			&handle_id, &radius ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = OilPaintImage( handle->image, radius, &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

                RETURN_FALSE ;
        }

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

        RETURN_TRUE ;
}

PHP_FUNCTION( imagick_solarize )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        double     threshold ;          /* define the extent of solarization -
					   this value is between 0 and MaxRGB;
					   see phpinfo() for MaxRGB */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rd",
			&handle_id, &threshold ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        SolarizeImage( handle->image, threshold ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                RETURN_FALSE ;
        }

        RETURN_TRUE ;
}

PHP_FUNCTION( imagick_swirl )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        double     degrees ;            /* define the tightness of the swirling
					   effect */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, swirled image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rd",
			&handle_id, &degrees ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = SwirlImage( handle->image, degrees, &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

                RETURN_FALSE ;
        }

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

        RETURN_TRUE ;
}

PHP_FUNCTION( imagick_wave )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        double     amplitude ;          /* the amplitude of the wave */
        double     frequency ;          /* the frequency of the wave */
        imagick_t* handle ;             /* the actual magick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, waved image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdd",
			&handle_id, &amplitude, &frequency ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = WaveImage( handle->image, amplitude, frequency,
			       &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

                RETURN_FALSE ;
        }

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

        RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Image effects.
 *
 */

PHP_FUNCTION( imagick_blur )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        double     radius ;           	/* the radius of the Gaussian, in
					   pixels, not counter the center
					   pixel */
        double     sigma ;           	/* the standard deviation of the
					   Gaussian, in pixels */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, blurred image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdd",
			&handle_id, &radius, &sigma ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = BlurImage( handle->image, radius, sigma,
			       &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

                RETURN_FALSE ;
        }

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

        RETURN_TRUE ;
}

PHP_FUNCTION( imagick_despeckle )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, despeckled image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = DespeckleImage( handle->image, &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

        RETURN_TRUE ;
}

PHP_FUNCTION( imagick_edge )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
	double     radius ;		/* the radius of the pixel neighbor-
					   hood; using 0 will cause a
					   suitable radius to be selected
					   for you */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rd",
			&handle_id, &radius ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = EdgeImage( handle->image, radius, &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

        RETURN_TRUE ;
}

PHP_FUNCTION( imagick_emboss )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
	double     radius ;		/* the radius of the pixel neighbor-
					   hood */
	double     sigma ;		/* the standard deviation of the
					   Gaussian, in pixels */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, embossed image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdd",
			&handle_id, &radius, &sigma ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = EmbossImage( handle->image, radius, sigma,
				 &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

        RETURN_TRUE ;
}

PHP_FUNCTION( imagick_enhance )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, enhanced image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = EnhanceImage( handle->image, &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_gaussianblur )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
	double     radius ;		/* the radius of the Gaussian, in
				 	   pixels, not counting the center
					   pixel; using 0 will cause a
					   suitable radius to be selected
					   for you */
	double     sigma ;		/* the standard deviation of the
					   Gaussian, in pixels */ 
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, Gaussian blurred image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdd",
			&handle_id, &radius, &sigma ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = GaussianBlurImage( handle->image, radius, sigma,
				       &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_medianfilter )
{
        zval*      handle_id ;           /* the handle identifier coming from
                                            the PHP environment */
	double     radius ;		 /* the radius of the pixel neighbor-
					    hood */
        imagick_t* handle ;              /* the actual imagick_t struct for the
                                            handle */
        Image*     new_image ;           /* the new, median filtered image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rd",
			&handle_id, &radius ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = MedianFilterImage( handle->image, radius,
				       &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_motionblur )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
	double     radius ;		/* the radius of the Gaussian, in
					   pixels, not counter the center
					   pixel */
	double     sigma ;		/* the standard deviation of the
					   Gaussian, in pixels */
	double     angle ;		/* apply the effect along this angle */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, motion blurred image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rddd",
			&handle_id, &radius, &sigma, &angle ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = MotionBlurImage( handle->image, radius, sigma,
				     angle, &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_reducenoise )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
	double     radius ;		/* the radius of the pixel
					   neighborhood */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, noise reduced image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rd",
			&handle_id, &radius ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = ReduceNoiseImage( handle->image, radius,
				      &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_shade )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
	long       gray ;		/* a value other than zero shades the
					   intensity of each pixel */
	double     azimuth ;		/* define light source direction */
	double     elevation ;		/* define light source direction */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, shaded image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rldd",
			&handle_id, &gray, &azimuth, &elevation ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = ShadeImage( handle->image, gray, azimuth, elevation,
				&handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_sharpen )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
	double     radius ;		/* the radius of the Gaussian, in
					   pixels, not counting the center
					   pixel */
	double     sigma ;		/* the standard deviation of the
					   Laplacian, in pixels */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, sharpened image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdd",
			&handle_id, &radius, &sigma ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = SharpenImage( handle->image, radius, sigma,
				  &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_spread )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
	long	   radius ;		/* choose a random pixel in a neighbor-
					   hood of this extent */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new, spread image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rl",
			&handle_id, &radius ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = SpreadImage( handle->image, radius, &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_threshold )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
	double     threshold ;		/* define the threshold value */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rd",
			&handle_id, &threshold ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        ThresholdImage( handle->image, threshold ) ;
        if ( _php_imagick_is_error( handle ) )
        {
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_unsharpmask )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
	double     radius ;		/* the radius of the Gaussian, in
					   pixels, not counting the center
					   pixel */
	double     sigma ;		/* the standard deviation of the
					   Gaussian, in pixels */
	double     amount ;		/* the percentage of the difference
					   between the original and the blur
				 	   blur image that is added back into
					   the original */
	double     threshold ;		/* the threshold in pixels needed to
					   apply the difference amount */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rdddd",
			&handle_id, &radius, &sigma, &amount, &threshold )
					== FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = UnsharpMaskImage( handle->image, radius, sigma, amount,
				      threshold, &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_flatten )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
        Image*     new_image ;          /* the new image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

        new_image = FlattenImages( handle->image, &handle->exception ) ;
        if ( _php_imagick_is_error( handle ) )
        {
                if ( new_image )
                {
                        DestroyImage( new_image ) ;
                }

		RETURN_FALSE ;
	}

        DestroyImage( handle->image ) ;
        handle->image = new_image ;

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Image list manipulation.
 *
 */

PHP_FUNCTION( imagick_next )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
	Image*     next_image ;		/* the next image in the list if it
					   exists */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

	next_image = GetNextImage( handle->image ) ;
	if ( next_image == ( Image* )NULL )
	{
		RETURN_FALSE ;
	}

	handle->image = next_image ;
	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_prev )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

	/*****

	   ImageMagick did not have a previous image function built in
	   until 5.5.2.  Since we support older versions, we'll just write
	   our own instead of relying on theirs.  In the future, this code
	   should be replaced with the native previous function written into
	   ImageMagick > 5.5.2.

	*****/

	if ( handle->image == ( Image* )NULL )
	{
		RETURN_FALSE ;
	}

	if ( handle->image->signature != MagickSignature )
	{
		php_error( E_WARNING, "%s(): image signature does not match MagickSignature", get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
	}

	if ( handle->image->previous == ( Image* )NULL )
	{
		RETURN_FALSE ;
	}

	handle->image = handle->image->previous ;
	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_first )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

	if ( !_php_imagick_first_image_in_list( handle ) )
	{
		RETURN_FALSE ;
	}
	
	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_goto )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
	long       frame ;		/* the frame to go to */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
	int        counter ;		/* generic loop counter */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rl",
			&handle_id, &frame ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

	if ( frame < 0 )
	{
		RETURN_FALSE ;
	}

	if ( handle->image == ( Image* )NULL )
	{
		RETURN_FALSE ;
	}

	if ( handle->image->signature != MagickSignature )
	{
		php_error( E_WARNING, "%s(): image signature does not match MagickSignature", get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
	}

	if ( GetImageListSize( handle->image ) < frame )
	{
		ThrowException( &handle->exception, WarningException, "image list does not contain that many frames in imagick_goto()", NULL ) ;
		RETURN_FALSE ;
	}

	/*****

	   First, let's go back to the first image in the list.

	****/

	_php_imagick_first_image_in_list( handle ) ;

	for ( counter = 0 ; counter < frame ; counter++ )
	{
		handle->image = handle->image->next ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_getlistsize )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
        {
                return ;
	}

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

	RETURN_LONG( GetImageListSize( handle->image ) ) ;
}

PHP_FUNCTION( imagick_getlistindex )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
        {
                return ;
	}

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

	RETURN_LONG( GetImageListIndex( handle->image ) ) ;
}

PHP_FUNCTION( imagick_getimagefromlist )
{
        zval*      handle_id ;          /* the handle identifier coming from
                                           the PHP environment */
        imagick_t* handle ;             /* the actual imagick_t struct for the
                                           handle */
	imagick_t* new_handle ;		/* the new handle containing the
					   image */
	
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
        {
                return ;
	}

        handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
        if ( !handle )
        {
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
                RETURN_FALSE ;
        }

        _php_imagick_clear_errors( handle ) ;

	new_handle = _php_imagick_alloc_handle() ;
	if ( !new_handle )
	{
		RETURN_FALSE ;
	}

	/*****

	   The point of this function is to extract JUST the image we're
	   currently point at.  So we'll only CloneImage() (not
	   CloneImageList()) and consider the image an orphan.

	*****/

	new_handle->image = CloneImage( handle->image, 0, 0, 1,
					&new_handle->exception ) ;

	/*****

	   _php_imagick_alloc_handle() already created a NULL image_info
	   structure.  We want to safely clean that up before assigning the
	   new one.

	*****/

	if ( new_handle->image_info )
	{
		DestroyImageInfo( new_handle->image_info ) ;
	}
	new_handle->image_info = CloneImageInfo( handle->image_info ) ;

	new_handle->id = zend_list_insert( new_handle, le_imagick_handle ) ;
	RETURN_RESOURCE( new_handle->id ) ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Transform an image.
 *
 */

PHP_FUNCTION( imagick_chop )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	long	      coord_x ;		/* x starting position of border */
	long	      coord_y ;		/* y starting position of border */
	long	      width ;		/* width of border */
	long	      height ;		/* height of border */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */
	RectangleInfo chop_info ;	/* struct containing x, y, width and
					   height of border */
	Image*        new_image ;	/* the new image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rllll",
			&handle_id, &coord_x, &coord_y, &width, &height )
				== FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	chop_info.width  = width ;
	chop_info.height = height  ;
	chop_info.x      = coord_x ;
	chop_info.y      = coord_y ;

	new_image = ChopImage( handle->image, &chop_info, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_crop )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	long	      coord_x ;		/* x starting position of border */
	long	      coord_y ;		/* y starting position of border */
	long	      width ;		/* width of border */
	long	      height ;		/* height of border */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */
	RectangleInfo geometry ;	/* struct containing x, y, width and
					   height of border */
	Image*        new_image ;	/* the new image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rllll",
			&handle_id, &coord_x, &coord_y, &width, &height )
				== FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	geometry.width  = width ;
	geometry.height = height  ;
	geometry.x      = coord_x ;
	geometry.y      = coord_y ;

	new_image = CropImage( handle->image, &geometry, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_flip )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*        new_image ;	/* the new image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	new_image = FlipImage( handle->image, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_flop )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*        new_image ;	/* the new image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	new_image = FlopImage( handle->image, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_roll )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	long	      x_offset ;	/* the number of cols to roll in the
					   horizontal direction */
	long          y_offset ;	/* the number of rows to roll in the
					   vertical direction */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */
	Image*        new_image ;	/* the new image */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rll",
			&handle_id, &x_offset, &y_offset) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	new_image = RollImage( handle->image, x_offset, y_offset,
			       &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		if ( new_image )
		{
			DestroyImage( new_image ) ;
		}

		RETURN_FALSE ;
	}

	DestroyImage( handle->image ) ;
	handle->image = new_image ;

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_profile )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	char*      name ;		/* the name of the profile to affect;
					   specifying * will affect all -
					   ICM, IPTC, generic profile */
	int        name_len ;		/* string length of name */
	int	   clone ;		/* if true, copy the profile rather
					   than taking ownership of it */
	char*	   profile ;		/* the profile; specifying NULL will
					   remove the profile */
	int        profile_len ;	/* string length of profile */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	profile     = NULL ;
	profile_len = 0 ;

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rsl|s",
			&handle_id, &name, &name_len, &clone, &profile,
		 	&profile_len ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	ProfileImage( handle->image, name, profile, profile_len, clone ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_rgbtransform )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	long       colorspace ;		/* the colorspace to transform the
					   image to */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rl",
			&handle_id, &colorspace ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	RGBTransformImage( handle->image, ( ColorspaceType )colorspace ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

PHP_FUNCTION( imagick_transformrgb )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	long       colorspace ;		/* the colorspace to transform the
					   image to */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rl",
			&handle_id, &colorspace ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	TransformRGBImage( handle->image, ( ColorspaceType )colorspace ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Quantize an image.
 *
 */

PHP_FUNCTION( imagick_ordereddither )
{
	zval* 	      handle_id ;	/* the handle identifier coming from
					   the PHP environment */
	imagick_t*    handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle ) ;

	OrderedDitherImage( handle->image ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Composite an image.
 *
 */

PHP_FUNCTION( imagick_composite )
{
	zval* 	      handle_id_1 ;	/* the handle identifier coming from
					   the PHP environment */
	long          composite_type ;  /* one of IMAGICK_COMPOSITE_OP_* */
	zval* 	      handle_id_2 ;	/* the handle identifier coming from
					   the PHP environment */
	long          x_offset ;	/* the column offset of the composited
					   image */
	long          y_offset ;	/* the row offset of the composited
					   image */
	imagick_t*    handle_1 ;	/* the actual imagick_t struct for the
					   handle */
	imagick_t*    handle_2 ;	/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rlrll",
			&handle_id_1, &composite_type, &handle_id_2,
			&x_offset, &y_offset ) == FAILURE )
	{
		return ;
	}

	handle_1 = _php_imagick_get_handle_struct_from_list( &handle_id_1 TSRMLS_CC ) ;
	if ( !handle_1 )
	{
		php_error( E_WARNING, "%s(): handle 1 is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	handle_2 = _php_imagick_get_handle_struct_from_list( &handle_id_2 TSRMLS_CC ) ;
	if ( !handle_2 )
	{
		php_error( E_WARNING, "%s(): handle 2 is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	_php_imagick_clear_errors( handle_1 ) ;
	_php_imagick_clear_errors( handle_2 ) ;

	CompositeImage( handle_1->image, ( CompositeOperator )composite_type,
			handle_2->image, x_offset, y_offset ) ;
	if ( _php_imagick_is_error( handle_1 ) ||
	     _php_imagick_is_error( handle_2 ) )
	{
		RETURN_FALSE ;
	}

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Clean up.
 *
 */

PHP_FUNCTION( imagick_destroyhandle )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle_id ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	/*****

	   zend_list_delete() will take care of the memory allocated for
	   the handle.

	*****/

	zend_list_delete( handle->id ) ;

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Internal functions.
 *
 */

static void _php_imagick_init( void )
{
	TSRMLS_FETCH();
	if ( IMAGICK_G( imagick_was_init ) == 1 )
	{
		return ;
	}

	InitializeMagick( NULL ) ;

	/*****

	   If you want to see the ImageMagick engine exception messages
	   output to stderr, change IMAGICK_TURN_OFF_ERRORS_TO_STDERR to
	   0 in ./imagick.h.

	*****/

	if ( IMAGICK_TURN_OFF_ERRORS_TO_STDERR == 1 )
	{
		SetErrorHandler( _php_imagick_error_handler ) ;
	}

	IMAGICK_G( imagick_was_init ) = 1 ;
}

static imagick_t* _php_imagick_alloc_handle( void )
{
	imagick_t* handle ;		/* the new handle to create */
					
	TSRMLS_FETCH();

	handle = ( imagick_t* )emalloc( sizeof( imagick_t ) ) ;
	if ( !handle )
	{
		php_error( E_ERROR, "%s(): could not allocate imagick_t structure for new image handle", get_active_function_name( TSRMLS_C ) ) ;
		return ( imagick_t* )NULL ;	
	}

	/*****

	   Set all structure members to default values.  POINTERS SHOULD BE
	   SET TO NULL and _php_imagick_clean_up_handle() should be modified
	   to clean up any additional members added.

	*****/

	handle->id            = -1 ;
	handle->image         = NULL ;
	handle->image_info    = CloneImageInfo( ( ImageInfo* )NULL ) ;
	GetExceptionInfo( &handle->exception ) ;
	handle->draw_info     = NULL ;
	handle->draw_context  = NULL ;

	return handle ;
}

static void _php_imagick_clean_up_handle( imagick_t* handle )
{
	if ( !handle )
	{
		return ;
	}

	if ( handle->image )
	{
		DestroyImageList( handle->image ) ;
	}

	if ( handle->image_info )
	{
		DestroyImageInfo( handle->image_info ) ;
	}

	DestroyExceptionInfo( &handle->exception ) ;

	if ( handle->draw_info )
	{
		DestroyDrawInfo( handle->draw_info ) ;
	}

	if ( handle->draw_context )
	{
		DrawDestroyContext( handle->draw_context ) ;
	}

	efree( handle ) ;
}

static void _php_imagick_free_handle_list( zend_rsrc_list_entry* rsrc TSRMLS_DC )
{
	imagick_t* handle ;		/* the handle to free */

	handle = ( imagick_t* )rsrc->ptr ;
	if ( handle )
	{
		_php_imagick_clean_up_handle( handle ) ;
	}
}

static imagick_t* _php_imagick_get_handle_struct_from_list( zval** handle_id TSRMLS_DC )
{
	imagick_t* handle ;		/* the imagick_t handle - used to
					   perform any ops. on the image */	

	handle = ( imagick_t* )zend_fetch_resource( handle_id TSRMLS_CC, -1,
				"imagemagick handle", NULL, 1,
				le_imagick_handle ) ;
	if ( !handle )
	{
		return ( imagick_t* )NULL ;
	}

	return handle ;
}

void _php_imagick_error_handler( const ExceptionType exception, const char* a,
                                 const char* b )
{
	/*****

	   We don't want anything being displayed to stderr.  So we'll
	   do nothing with errors assuming they will be properly assigned
	   to an ExceptionInfo structure.

	*****/
}

static void _php_imagick_clear_errors( imagick_t* handle )
{
	if ( !handle )
	{
		return ;
	}

	handle->exception.severity        = UndefinedException ;
	handle->image->exception.severity = UndefinedException ;

	/*****

	   I don't actually free the memory allocated for the reason and
	   description because I don't know how ImageMagick is going to use
	   it.  ImageMagick might declare the reason and description to
	   a fixed length and then just strncpy() messages into the
	   memory.  So to be safe, we'll just leave the memory intact.

	*****/
}

static int _php_imagick_is_error( const imagick_t* handle )
{
	if ( handle->exception.severity        != UndefinedException ||
	     handle->image->exception.severity != UndefinedException )
	{
		return 1 ;
	}

	return 0 ;
}

/*
 *  This function is here to facilitate supporting backwards compatible
 *  functions, namely imagick_read().  If the backwards compatibility
 *  functions are removed, the statements in this function can be moved
 *  into image_readimage() directly.
*/
static imagick_t* _php_imagick_readimage( const char* file_name )
{
	imagick_t* handle ;		/* the new image handle */

	TSRMLS_FETCH();					
	if ( !file_name )
	{
		return NULL ;
	}

	if ( IMAGICK_G( imagick_was_init ) == 0 )
	{
		/*****

		   Initialize the ImageMagick engine.

		*****/

		_php_imagick_init() ;
	}

	handle = _php_imagick_alloc_handle() ;
	if ( !handle )
	{
		return NULL ;	
	}
	handle->id = zend_list_insert( handle, le_imagick_handle ) ;

	strncpy( handle->image_info->filename, file_name, MaxTextExtent - 1 ) ;

	handle->image = ReadImage( handle->image_info, &handle->exception ) ;
	if ( _php_imagick_is_error( handle ) )
	{
		return handle ;
	}

	if ( !handle->image )
	{
		ThrowException( &handle->exception, FatalErrorException, "after ReadImage(), image is empty in _php_imagick_readimage()", NULL ) ;
		return handle ;
	}

	return handle ;
}

static int _php_imagick_first_image_in_list( imagick_t* handle )
{
	TSRMLS_FETCH();					

	if ( !handle )
	{
		return 0 ;
	}

	if ( handle->image == ( Image* )NULL )
	{
		return 0 ;
	}

	if ( handle->image->signature != MagickSignature )
	{
		php_error( E_WARNING, "%s(): image signature does not match MagickSignature", get_active_function_name( TSRMLS_C ) ) ;
                return 0 ;
	}

	while( handle->image->previous != ( Image* )NULL )
	{
		handle->image = handle->image->previous ;
	}

	return 1 ;
}

static int _php_imagick_get_geometry_rect( imagick_t* handle, long cols,
					   long rows, char* mods,
					   RectangleInfo* rect_info )
{
	char* geometry ;		/* the ImageMagick standards geometry
					   string */

	geometry = ( char* )emalloc( MaxTextExtent ) ;
	if ( !geometry )
	{
		return 0 ;
	}

	if ( mods )
	{
		snprintf( geometry, MaxTextExtent, "%ldx%ld%s\0", cols, rows,
			  mods ) ;
		GetImageGeometry( handle->image, geometry, 1, rect_info ) ;
	}
	else
	{
		snprintf( geometry, MaxTextExtent, "%ldx%ld\0", cols, rows ) ;
		GetImageGeometry( handle->image, geometry, 1, rect_info ) ;
	}

	if ( geometry )
	{
		efree( geometry ) ;
	}

	return 1 ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Backward compatibility with old imagick extension.
 *
 */

PHP_FUNCTION( imagick_read )
{
        zval*      handle_id ;           /* this is not used here and is
					   maintained for backward
					   compatibility only */
	char* 	   file_name ;		/* the file name to use to retrieve
					   the image */
	int        file_name_len ;	/* string length of the file name */
	imagick_t* handle ;		/* the handle for this image */ 

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "rs",
			&handle_id, &file_name, &file_name_len ) == FAILURE )
        {
                return ;
        }

        handle = _php_imagick_readimage( file_name ) ;
	RETURN_RESOURCE( handle->id ) ;
}

PHP_FUNCTION( imagick_free )
{
	zval* 	   handle_id ;		/* the handle identifier coming from
					   the PHP environment */
	imagick_t* handle ;		/* the actual imagick_t struct for the
					   handle */

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "r",
			&handle ) == FAILURE )
	{
		return ;
	}

	handle = _php_imagick_get_handle_struct_from_list( &handle_id TSRMLS_CC ) ;
	if ( !handle )
	{
		php_error( E_WARNING, "%s(): handle is invalid",
			   get_active_function_name( TSRMLS_C ) ) ;
		RETURN_FALSE ;
	}

	/*****

	   zend_list_delete() will take care of the memory allocated for
	   the handle.

	*****/

	zend_list_delete( handle->id ) ;

	RETURN_TRUE ;
}

/*****************************************************************************/

/******************************************************************************
 *
 *  Deprecated functions from old imagick extension.
 *
 */

PHP_FUNCTION( imagick_add_resource )
{
	IMAGICK_DEPCREATED() ;
	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_list_magickinfo )
{
	IMAGICK_DEPCREATED() ;
	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_new )
{
	IMAGICK_DEPCREATED() ;
	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_init )
{
	IMAGICK_DEPCREATED() ;
	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_copy_sample )
{
	IMAGICK_DEPCREATED() ;
	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_copy_resize )
{
	IMAGICK_DEPCREATED() ;
	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_copy_crop )
{
	IMAGICK_DEPCREATED() ;
	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_copy_shear )
{
	IMAGICK_DEPCREATED() ;
	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_copy_rotate )
{
	IMAGICK_DEPCREATED() ;
	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_copy_morph )
{
	IMAGICK_DEPCREATED() ;
	RETURN_FALSE ;
}

PHP_FUNCTION( imagick_dump )
{
	IMAGICK_DEPCREATED() ;
	RETURN_FALSE ;
}

/*****************************************************************************/
