/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Imagick	                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 2006-2009 Mikko Koppanen, Scott MacVicar               |
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

#include "php_imagick.h"
#include "php_imagick_defs.h"
#include "php_imagick_macros.h"

/* {{{ Indicates whether image filename has a page. For example test.pdf[0] */
zend_bool php_imagick_has_page(char *filename, int filename_len TSRMLS_DC) {
	/* Too short */
	if (filename_len < 3) {
		return 0;
	}
	/* If these conditions match it's safe to assume the filename contains a page */
	if ((filename[filename_len - 1] == ']') && 
		(filename[filename_len - 2] >= 48) && 
		(filename[filename_len - 2] <= 57)) {
		return 1;
	}
	return 0;
}
/* }}} */

/* {{{ url format? */
zend_bool php_imagick_is_url(char *filename, int filename_len TSRMLS_DC)
{
	if (strncasecmp(filename, "http://", 7) == 0 ||
		strncasecmp(filename, "https://", 8) == 0 || 
		strncasecmp(filename, "ftp://", 6) == 0 ||
		strncasecmp(filename, "ftps://", 7) == 0) {
		return 1;	
	}
	return 0;
}
/* }}} */

/* {{{ check if recognised format */
int php_imagick_recognized_format(char *filename, int filename_len TSRMLS_DC)
{
	char **formats, *format, *pch;
	unsigned long chr_pos, i, num_formats = 0;
	
	pch = strchr(filename, ':');
	
	if (pch == NULL) {
		return -1;
	} 
	
	chr_pos = pch - filename;
	format = estrndup(filename, chr_pos);

	if (strcasecmp(format, "MAGICK") == 0) {
		efree(format);
		return chr_pos;
	}

	formats = MagickQueryFormats(php_strtoupper(format, chr_pos), &num_formats);
	efree(format);

	if (num_formats == 0) {
		chr_pos = -1;
	}
	
	for (i = 0 ; i < num_formats ; i++) {
		IMAGICK_FREE_MEMORY(char *, formats[i]);
	}
	
	IMAGICK_FREE_MEMORY(char **, formats);
	return chr_pos;
}
/* }}} */

/* {{{ Does the filename have a format indicator? 
		Returns format length if found, -1 on fail */
int php_imagick_format_indicator(char *filename, int filename_len TSRMLS_DC) 
{
	if (count_occurences_of(':', filename TSRMLS_CC) == 0) {
		return -1;
	}
	return php_imagick_recognized_format(filename, filename_len TSRMLS_CC);
}
/* }}} */

/* {{{ Is the format virtual ? */
zend_bool php_imagick_is_virtual_format(char *filename, int filename_len TSRMLS_DC) {
#ifdef PHP_WIN32
	const int elements = 18;
	int i = 0;
	const char *no_basedir_fmt[] = { "CAPTION:", "CLIPBOARD:", "FRACTAL:", "GRADIENT:", "LABEL:", "MATTE:",
									"NULL:", "PLASMA:", "PRINT:", "SCAN:", "RADIAL_GRADIENT:", "SCANX:",
									"WIN:", "XC:", "MAGICK:", "GRANITE:", "LOGO:", "NETSCAPE:", "ROSE:" };
#else
	const int elements = 19;
	int i = 0;
	const char *no_basedir_fmt[] = { "CAPTION:", "CLIPBOARD:", "FRACTAL:", "GRADIENT:", "LABEL:", "MATTE:",
									"NULL:", "PLASMA:", "PRINT:", "SCAN:", "RADIAL_GRADIENT:", "SCANX:",
									"WIN:", "X:", "XC:", "MAGICK:", "GRANITE:", "LOGO:", "NETSCAPE:", "ROSE:" };
#endif
	for (i = 0; i < elements; i++) {
		if (strncasecmp(filename, no_basedir_fmt[i], strlen(no_basedir_fmt[i])) == 0) {
			return 1;
		}
	}
	return 0;
}
/* }}} */

/* {{{ Gets absolute filename of the file.
	   Returns null if format is virtual 
	   Filename can be passed with or without format indicator */
char *php_imagick_get_absolute_filename(char *filename, int filename_len TSRMLS_DC)
{
	int chr_pos = php_imagick_format_indicator(filename, filename_len TSRMLS_CC);

	if (chr_pos == -1) {
		/* There was no recognised format in the filename, try to access as is */
		return expand_filepath(filename, NULL TSRMLS_CC);
	}
	
	/* the was a format but is it virtual?? */
	if (php_imagick_is_virtual_format(filename, filename_len TSRMLS_CC)) {
		/* virtual format has no filename */
		return NULL;
	}
	
	return expand_filepath(filename + chr_pos + 1, NULL TSRMLS_CC);
}
/* }}} */

/* {{{ Whether php stream should be used to read the image */
zend_bool php_imagick_use_stream(char *filename, int filename_len TSRMLS_DC)
{
	/* Read urls using php */
	if (php_imagick_is_url(filename, filename_len TSRMLS_CC)) {
		return 1;
	}
	
	if ((php_imagick_format_indicator(filename, filename_len TSRMLS_CC) != -1) ||
		(php_imagick_has_page(filename, filename_len TSRMLS_CC))) {
		return 0;
	}
	
	/* The rest using php streams */
	return 1;
}
/* }}} */ 

int php_imagick_safety_check(char *filename, int filename_len TSRMLS_DC) 
{
	int status = IMAGICK_READ_WRITE_NO_ERROR;

	if (PG(open_basedir) || PG(safe_mode)) {
		char *absolute = php_imagick_get_absolute_filename(filename, filename_len TSRMLS_CC);
				
		if (absolute) {
			
			if (PG(safe_mode) && (!php_checkuid_ex(absolute, NULL, CHECKUID_CHECK_FILE_AND_DIR, CHECKUID_NO_ERRORS))) {
				status = IMAGICK_READ_WRITE_SAFE_MODE_ERROR;
			}
			
			if (PG(open_basedir) && php_check_open_basedir_ex(absolute, 0 TSRMLS_CC)) {
				status = IMAGICK_READ_WRITE_OPEN_BASEDIR_ERROR;
			}
			
			efree(absolute);
		}
	}
	return status;
}

/* {{{ }}} */
MagickBooleanType php_imagick_read_image_using_imagemagick(php_imagick_object *intern, int type, char *filename, int filename_len TSRMLS_DC)
{
	int status;
	char *absolute;
	
	status = php_imagick_safety_check(filename, filename_len TSRMLS_CC);
	
	if (status != IMAGICK_READ_WRITE_NO_ERROR) { 
		return status;
	}

	if (type == 1) {
		if (MagickReadImage(intern->magick_wand, filename) == MagickFalse) {
			return IMAGICK_READ_WRITE_UNDERLYING_LIBRARY;
		}	
	} else {
		if (MagickPingImage(intern->magick_wand, filename) == MagickFalse) {
			return IMAGICK_READ_WRITE_UNDERLYING_LIBRARY;
		}
	}

	absolute = php_imagick_get_absolute_filename(filename, filename_len TSRMLS_CC);
	
	if (!absolute) {
		absolute = estrdup("");
	}

	MagickSetImageFilename(intern->magick_wand, absolute);
	efree(absolute);

	IMAGICK_CORRECT_ITERATOR_POSITION(intern);
	return IMAGICK_READ_WRITE_NO_ERROR;
}


int php_imagick_read_image_using_php_streams(php_imagick_object *intern, int type, char *filename, int filename_len TSRMLS_DC)
{
	php_stream *stream;
	MagickBooleanType status;
	FILE *fp;
#if ZEND_MODULE_API_NO > 20060613 
	zend_error_handling error_handling;
#endif
	
#if ZEND_MODULE_API_NO > 20060613 
	zend_replace_error_handling(EH_THROW, php_imagick_exception_class_entry, &error_handling TSRMLS_CC);
#else
	php_set_error_handling(EH_THROW, php_imagick_exception_class_entry TSRMLS_CC);
#endif
	
	stream = php_stream_open_wrapper(filename, "rb", (ENFORCE_SAFE_MODE|IGNORE_PATH) & ~REPORT_ERRORS, NULL);

	if (!stream) {
		goto return_error;
	}
	
	if (php_stream_can_cast(stream, PHP_STREAM_AS_STDIO|PHP_STREAM_CAST_INTERNAL) == FAILURE) {
		goto return_error;
	}

	if (php_stream_cast(stream, PHP_STREAM_AS_STDIO|PHP_STREAM_CAST_INTERNAL, (void*)&fp, 0) == FAILURE) {
		goto return_error;
	}
	
#if ZEND_MODULE_API_NO > 20060613 
	zend_restore_error_handling(&error_handling TSRMLS_CC);
#else
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
#endif

	if (type == 1) {
		status = MagickReadImageFile(intern->magick_wand, fp);
	} else {
		status = MagickPingImageFile(intern->magick_wand, fp);
	}
	
	if (php_stream_is(stream, PHP_STREAM_IS_STDIO)) {
		char *absolute = expand_filepath(filename, NULL TSRMLS_CC);
		MagickSetImageFilename(intern->magick_wand, absolute);
		efree(absolute);
	} else {
		/* Set to empty filename, otherwise it will point to MAGICK_TEMP/magick-XXXXX */
		MagickSetImageFilename(intern->magick_wand, "");
	}
	php_stream_close(stream);

	if (status == MagickFalse) {
		return IMAGICK_READ_WRITE_UNDERLYING_LIBRARY;
	}
	
	IMAGICK_CORRECT_ITERATOR_POSITION(intern);
	return IMAGICK_READ_WRITE_NO_ERROR;
	
return_error:
#if ZEND_MODULE_API_NO > 20060613 
	zend_restore_error_handling(&error_handling TSRMLS_CC);
#else
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
#endif
	if (stream) php_stream_close(stream);
	return IMAGICK_READ_WRITE_UNDERLYING_LIBRARY;
}

int read_image_into_magickwand(php_imagick_object *intern, int type, char *filename, int filename_len TSRMLS_DC)
{	
	if (!filename) {
		return IMAGICK_READ_WRITE_NO_ERROR;
	}

	/* Use traditional imagemagick method or php streams (?) */
	if (php_imagick_use_stream(filename, filename_len TSRMLS_CC)) {
		 return php_imagick_read_image_using_php_streams(intern, type, filename, filename_len TSRMLS_CC);
	} else {	
		return php_imagick_read_image_using_imagemagick(intern, type, filename, filename_len TSRMLS_CC);
	}	
}
