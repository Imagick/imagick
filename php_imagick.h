/*
  +----------------------------------------------------------------------+
  | PHP Version 4                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2002 The PHP Group                                |
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

#ifndef PHP_IMAGICK_H
#define PHP_IMAGICK_H

	extern zend_module_entry imagick_module_entry ;
	#define phpext_imagick_ptr &imagick_module_entry

	#ifdef PHP_WIN32
		#define PHP_IMAGICK_API __declspec( dllexport )
	#else
		#define PHP_IMAGICK_API
	#endif

	#ifdef ZTS
		#include "TSRM.h"
	#endif

	ZEND_BEGIN_MODULE_GLOBALS( imagick )
		int imagick_was_init ;
	ZEND_END_MODULE_GLOBALS( imagick )

	#ifdef ZTS
		# define IMAGICK_G( v ) TSRMG( imagick_globals_id, zend_imagick_globals *, v )
	#else
		# define IMAGICK_G( v ) ( imagick_globals.v )
	#endif

	PHP_MINIT_FUNCTION( imagick ) ;
	PHP_RINIT_FUNCTION( imagick ) ;
	PHP_RSHUTDOWN_FUNCTION( imagick ) ;
	PHP_MINFO_FUNCTION( imagick ) ;

	/*****

   	   Constitue an image.

	*****/

	PHP_FUNCTION( imagick_readimage ) ;
	PHP_FUNCTION( imagick_writeimage ) ;
	PHP_FUNCTION( imagick_writeimages ) ;
	PHP_FUNCTION( imagick_clonehandle) ;
	PHP_FUNCTION( imagick_image2blob ) ;
	PHP_FUNCTION( imagick_blob2image ) ;
	PHP_FUNCTION( imagick_getcanvas ) ;

	/*****

   	   Error handling.

	*****/

	PHP_FUNCTION( imagick_iserror ) ;
	PHP_FUNCTION( imagick_failedreason ) ;
	PHP_FUNCTION( imagick_faileddescription ) ;

	/*****

	   Getting information about an image.

	*****/

	PHP_FUNCTION( imagick_getwidth ) ;
	PHP_FUNCTION( imagick_getheight ) ;
	PHP_FUNCTION( imagick_getmimetype ) ;
	PHP_FUNCTION( imagick_getimagedepth ) ;
	PHP_FUNCTION( imagick_getnumbercolors ) ;
	PHP_FUNCTION( imagick_isgrayimage ) ;
	PHP_FUNCTION( imagick_ismonochromeimage ) ;
	PHP_FUNCTION( imagick_isopaqueimage ) ;
	PHP_FUNCTION( imagick_ispaletteimage ) ;

	/*****

   	   Convert an image.

	*****/

	PHP_FUNCTION( imagick_convert ) ;

	/*****

   	   Resize an image.

	*****/

	PHP_FUNCTION( imagick_resize ) ;
	PHP_FUNCTION( imagick_magnify ) ;
	PHP_FUNCTION( imagick_minify ) ;
	PHP_FUNCTION( imagick_scale ) ;
	PHP_FUNCTION( imagick_sample ) ;

	/*****

   	   Font settings.

	*****/

	PHP_FUNCTION( imagick_setfontsize ) ;
	PHP_FUNCTION( imagick_setfontstyle ) ;
	PHP_FUNCTION( imagick_setfontface ) ;

	/*****

   	   Draw on an image.

	*****/

	PHP_FUNCTION( imagick_begindraw ) ;
	PHP_FUNCTION( imagick_drawannotation ) ;
	PHP_FUNCTION( imagick_drawrectangle ) ;
	PHP_FUNCTION( imagick_drawellipse ) ;
	PHP_FUNCTION( imagick_drawarc ) ;
	PHP_FUNCTION( imagick_drawcircle ) ;
	PHP_FUNCTION( imagick_drawline ) ;
	PHP_FUNCTION( imagick_drawpoint ) ;
	PHP_FUNCTION( imagick_setfillcolor ) ;

	/*****

   	   Enhance an image.

	*****/

	PHP_FUNCTION( imagick_rotate ) ;
	PHP_FUNCTION( imagick_shear ) ;
	PHP_FUNCTION( imagick_contrast ) ;
	PHP_FUNCTION( imagick_equalize ) ;
	PHP_FUNCTION( imagick_gamma ) ;
	PHP_FUNCTION( imagick_level ) ;
	PHP_FUNCTION( imagick_modulate ) ;
	PHP_FUNCTION( imagick_negate ) ;
	PHP_FUNCTION( imagick_normalize ) ;

	/*****

	   Decorate an image.

	*****/

	PHP_FUNCTION( imagick_border ) ;
	PHP_FUNCTION( imagick_frame ) ;
	PHP_FUNCTION( imagick_raise ) ;

	/*****

	   Special effects.

	*****/

	PHP_FUNCTION( imagick_charcoal ) ;
	PHP_FUNCTION( imagick_implode ) ;
	PHP_FUNCTION( imagick_oilpaint ) ;
	PHP_FUNCTION( imagick_solarize ) ;
	PHP_FUNCTION( imagick_swirl ) ;
	PHP_FUNCTION( imagick_wave ) ;

	/*****

	   Image effects.

	*****/

	PHP_FUNCTION( imagick_blur ) ;
	PHP_FUNCTION( imagick_despeckle ) ;
	PHP_FUNCTION( imagick_edge ) ;
	PHP_FUNCTION( imagick_emboss ) ;
	PHP_FUNCTION( imagick_enhance ) ;
	PHP_FUNCTION( imagick_gaussianblur ) ;
	PHP_FUNCTION( imagick_medianfilter ) ;
	PHP_FUNCTION( imagick_motionblur ) ;
	PHP_FUNCTION( imagick_reducenoise ) ;
	PHP_FUNCTION( imagick_shade ) ;
	PHP_FUNCTION( imagick_sharpen ) ;
	PHP_FUNCTION( imagick_spread ) ;
	PHP_FUNCTION( imagick_threshold ) ;
	PHP_FUNCTION( imagick_unsharpmask ) ;

	/*****

	   Image list manipulation.

	*****/

	PHP_FUNCTION( imagick_next ) ;
	PHP_FUNCTION( imagick_prev ) ;
	PHP_FUNCTION( imagick_first ) ;
	PHP_FUNCTION( imagick_goto ) ;
	PHP_FUNCTION( imagick_getlistsize ) ;
	PHP_FUNCTION( imagick_getlistindex ) ;
	PHP_FUNCTION( imagick_getimagefromlist ) ;

	/*****

	   Transform an image.

	*****/

	PHP_FUNCTION( imagick_chop ) ;
	PHP_FUNCTION( imagick_crop ) ;
	PHP_FUNCTION( imagick_flip ) ;
	PHP_FUNCTION( imagick_flop ) ;
	PHP_FUNCTION( imagick_roll ) ;
	PHP_FUNCTION( imagick_profile ) ;

	/*****

	   Quantize an image.

	*****/

	PHP_FUNCTION( imagick_ordereddither ) ;

	/*****

	   Composite an image.

	*****/

	PHP_FUNCTION( imagick_composite ) ;

	/*****

	   Clean up.

	*****/

	PHP_FUNCTION( imagick_destroyhandle ) ;

	/*****

	   Backward compatibility with old imagick extension.

	*****/

	PHP_FUNCTION( imagick_read ) ;		/* => imagick_readimage() */
	PHP_FUNCTION( imagick_free ) ;		/* => imagick_destroyhandle() */	
	/*****

	   Deprecated functions from old imagick extension.

	*****/

	PHP_FUNCTION( imagick_add_resource ) ;
	PHP_FUNCTION( imagick_list_magickinfo ) ;
	PHP_FUNCTION( imagick_new ) ;
	PHP_FUNCTION( imagick_init ) ;
	PHP_FUNCTION( imagick_copy_sample ) ;
	PHP_FUNCTION( imagick_copy_resize ) ;
	PHP_FUNCTION( imagick_copy_crop ) ;
	PHP_FUNCTION( imagick_copy_shear ) ;
	PHP_FUNCTION( imagick_copy_rotate ) ;
	PHP_FUNCTION( imagick_copy_morph ) ;
	PHP_FUNCTION( imagick_dump ) ;

#endif	/* PHP_IMAGICK_H */
