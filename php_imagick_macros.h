/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Imagick	                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 2006-2013 Mikko Koppanen, Scott MacVicar               |
   | Imagemagick (c) ImageMagick Studio LLC                               |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Mikko Kopppanen <mkoppanen@php.net>                          |
   |         Scott MacVicar <scottmac@php.net>                            |
   +----------------------------------------------------------------------+
*/

#ifndef PHP_IMAGICK_MACROS_H
# define PHP_IMAGICK_MACROS_H

#define IMAGICK_ENSURE_NOT_EMPTY(magick_wand) \
	if(MagickGetNumberImages(magick_wand) == 0) { \
		php_imagick_throw_exception(IMAGICK_CLASS, "Can not process empty Imagick object" TSRMLS_CC); \
		return; \
	}

#define IMAGICK_FREE_MEMORY(type, value)\
	if (value != (type) NULL) { \
		value = (type) MagickRelinquishMemory(value); \
		value = (type)NULL; \
	} \

#define IMAGICK_METHOD_DEPRECATED(class_name, method_name) \
	php_error(E_STRICT, "%s::%s method is deprecated and it's use should be avoided", class_name, method_name);

#define IMAGICK_METHOD_DEPRECATED_USE_INSTEAD(class_name, method_name, new_class, new_method) \
	php_error(E_STRICT, "%s::%s is deprecated. %s::%s should be used instead", class_name, method_name, new_class, new_method);

#define IMAGICK_REPLACE_MAGICKWAND(intern, new_wand)\
	if (intern->magick_wand == NULL) {\
		intern->magick_wand = new_wand; \
	} else { \
		intern->magick_wand = DestroyMagickWand(intern->magick_wand); \
		intern->magick_wand = new_wand; \
	}

#define IMAGICKPIXEL_REPLACE_PIXELWAND(intern, new_wand)\
	if(intern->pixel_wand != NULL && intern->initialized_via_iterator != 1) {\
		intern->pixel_wand = DestroyPixelWand(intern->pixel_wand);\
		intern->pixel_wand = new_wand;\
	} else {\
		intern->pixel_wand = new_wand;\
	}

#define IMAGICKDRAW_REPLACE_DRAWINGWAND(intern, new_wand) \
	if (intern->drawing_wand == NULL) { \
		intern->drawing_wand = new_wand; \
	} else { \
		intern->drawing_wand = (DrawingWand *)DestroyDrawingWand(intern->drawing_wand); \
		intern->drawing_wand = new_wand; \
	}

#if MagickLibVersion >= 0x635
#define IMAGICK_CLONE_PIXELWAND(source, target) \
	target = ClonePixelWand(source);
#else
#define IMAGICK_CLONE_PIXELWAND(source, target) \
	target = (PixelWand *)NewPixelWand(); \
	PixelSetColorCount(target, PixelGetColorCount(source)); \
	PixelSetRed(target, PixelGetRed(source)); \
	PixelSetGreen(target, PixelGetGreen(source)); \
	PixelSetBlue(target, PixelGetBlue(source)); \
	PixelSetOpacity(target, PixelGetOpacity(source)); \
	PixelSetAlpha(target, PixelGetAlpha(source));
#endif

#endif /* PHP_IMAGICK_MACROS_H */
