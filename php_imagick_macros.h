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
		php_imagick_throw_exception(IMAGICK_CLASS, "Can not process empty Imagick object", 1 TSRMLS_CC); \
		return; \
	}

#define IMAGICK_FREE_MEMORY(type, value)\
	if (value != (type) NULL) { \
		value = (type) MagickRelinquishMemory(value); \
		value = (type)NULL; \
	} \

#if defined(CHECKUID_CHECK_FILE_AND_DIR)
#define IMAGICK_SAFE_MODE_CHECK(filename_, status_) \
	if (filename_) { \
		if (strlen(filename_) > MAXPATHLEN) { \
			status_ = IMAGICK_READ_WRITE_FILENAME_TOO_LONG; \
		} \
		if (PG(safe_mode) && (!php_checkuid_ex(filename_, NULL, CHECKUID_CHECK_FILE_AND_DIR, CHECKUID_NO_ERRORS))) { \
			status_ = IMAGICK_READ_WRITE_SAFE_MODE_ERROR; \
		} \
		if (php_check_open_basedir_ex(filename_, 0 TSRMLS_CC)) { \
			status_ = IMAGICK_READ_WRITE_OPEN_BASEDIR_ERROR; \
		} \
	}
#else
#define IMAGICK_SAFE_MODE_CHECK(filename_, status_)\
	if (filename_) { \
		if (strlen(filename_) > MAXPATHLEN) { \
			status_ = IMAGICK_READ_WRITE_FILENAME_TOO_LONG; \
		} \
		if (php_check_open_basedir_ex(filename_, 0 TSRMLS_CC)) { \
			status_ = IMAGICK_READ_WRITE_OPEN_BASEDIR_ERROR; \
		} \
	}
#endif

#define IMAGICK_METHOD_DEPRECATED(class_name, method_name) \
	php_error(E_STRICT, "%s::%s method is deprecated and it's use should be avoided", class_name, method_name);

#define IMAGICK_METHOD_DEPRECATED_USE_INSTEAD(class_name, method_name, new_class, new_method) \
	php_error(E_STRICT, "%s::%s is deprecated. %s::%s should be used instead", class_name, method_name, new_class, new_method);

#define IMAGICK_CHECK_READ_OR_WRITE_ERROR(intern, filename, error, free, message) \
	switch (error) { \
		case IMAGICK_READ_WRITE_NO_ERROR: \
			/* No error */ \
		break; \
		case IMAGICK_READ_WRITE_SAFE_MODE_ERROR:\
			zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, "Safe mode restricts user to read image: %s", filename); \
			if (free == IMAGICK_FREE_FILENAME && filename) { \
				 efree(filename); \
			} \
			RETURN_NULL();\
		break; \
		case IMAGICK_READ_WRITE_OPEN_BASEDIR_ERROR: \
			zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, "open_basedir restriction in effect. File(%s) is not within the allowed path(s)", filename); \
			if (free == IMAGICK_FREE_FILENAME && filename) { \
				 efree(filename); \
			} \
			RETURN_NULL(); \
		break; \
		default: \
		case IMAGICK_READ_WRITE_UNDERLYING_LIBRARY: \
			{ \
				ExceptionType severity; \
				char *description = MagickGetException(intern->magick_wand, &severity); \
				if (strlen(description) == 0) { \
					zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, message, filename);\
				} else {\
					zend_throw_exception(php_imagick_exception_class_entry, description, 1 TSRMLS_CC);\
					IMAGICK_FREE_MEMORY(char *, description);\
					MagickClearException(intern->magick_wand);\
					description = (char *)NULL;\
				}\
			}\
			if (free == IMAGICK_FREE_FILENAME && filename) { \
				 efree(filename); \
			} \
			RETURN_NULL();\
		case IMAGICK_READ_WRITE_PERMISSION_DENIED:\
			zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, "Permission denied to: %s", filename);\
			if (free == IMAGICK_FREE_FILENAME && filename) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
		case IMAGICK_READ_WRITE_FILENAME_TOO_LONG:\
			zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, "Filename too long: %s", filename);\
			if (free == IMAGICK_FREE_FILENAME && filename) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
		case IMAGICK_READ_WRITE_PATH_DOES_NOT_EXIST:\
			zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, "The path does not exist: %s", filename);\
			if (free == IMAGICK_FREE_FILENAME && filename) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
	} \

#define IMAGICKDRAW_CHECK_READ_OR_WRITE_ERROR(internd, filename, error, free)\
	switch (error) {\
		default:\
		case 0:\
			/* No error */\
		break;\
		case 1:\
			zend_throw_exception_ex(php_imagickdraw_exception_class_entry, 1 TSRMLS_CC, "Safe mode restricts user to read file: %s", filename);\
			if (free == IMAGICK_FREE_FILENAME && filename) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
		case 2:\
			zend_throw_exception_ex(php_imagickdraw_exception_class_entry, 1 TSRMLS_CC, "open_basedir restriction in effect. File(%s) is not within the allowed path(s)", filename);\
			if (free == IMAGICK_FREE_FILENAME && filename) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
		case 3:\
			IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to read file", 1);\
			if (free == IMAGICK_FREE_FILENAME && filename) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
	} \

#define IMAGICK_CORRECT_ITERATOR_POSITION(intern)\
	MagickSetLastIterator(intern->magick_wand);

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


#ifdef HAVE_LOCALE_H
#define IMAGICK_SET_LOCALE(old_locale, tmp, restore) \
	if ((IMAGICK_G(locale_fix)) && \
		(tmp = setlocale(LC_NUMERIC, NULL)) != NULL && \
		(strcmp(tmp, IMAGICK_LC_NUMERIC_LOCALE) != 0)) { \
		old_locale = estrdup(tmp); \
		setlocale(LC_NUMERIC, IMAGICK_LC_NUMERIC_LOCALE); \
		restore = 1; \
	} else { \
		restore = 0; \
	} \

#define IMAGICK_RESTORE_LOCALE(old_locale, restore) \
	if (restore && old_locale != NULL && (strcmp(old_locale, IMAGICK_LC_NUMERIC_LOCALE) != 0)) { \
 		setlocale(LC_NUMERIC, old_locale); \
		efree(old_locale); \
	} \

#else
#define IMAGICK_SET_LOCALE(old_locale, tmp, restore)
#define IMAGICK_RESTORE_LOCALE(old_locale, restore)
#endif

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
