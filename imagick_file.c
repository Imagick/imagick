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

#include "php_imagick.h"
#include "php_imagick_file.h"
#include "php_imagick_macros.h"
#include "php_imagick_defs.h"

#if ZEND_MODULE_API_NO > 20060613 
#  define IMAGICK_INIT_ERROR_HANDLING  zend_error_handling error_handling
#  define IMAGICK_SET_ERROR_HANDLING_THROW zend_replace_error_handling(EH_THROW, php_imagick_exception_class_entry, &error_handling TSRMLS_CC)
#  define IMAGICK_RESTORE_ERROR_HANDLING   zend_restore_error_handling(&error_handling TSRMLS_CC)
#else
#  define IMAGICK_INIT_ERROR_HANDLING
#  define IMAGICK_SET_ERROR_HANDLING_THROW php_set_error_handling(EH_THROW, php_imagick_exception_class_entry TSRMLS_CC)
#  define IMAGICK_RESTORE_ERROR_HANDLING   php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC)
#endif

#ifndef S_ISDIR
#  define S_ISDIR(mode) (((mode)&S_IFMT) == S_IFDIR)
#endif

static
zend_bool php_imagick_is_virtual_format(const char *format)
{
	int i, elements;

	const char *virtual_fmt[] = {
		"CAPTION",
		"CLIPBOARD",
		"FRACTAL",
		"GRADIENT",
		"LABEL",
		"MATTE",
		"NULL",
		"PLASMA",
		"PRINT",
		"SCAN",
		"RADIAL-GRADIENT",
		"SCANX",
		"WIN",
#ifndef PHP_WIN32
		"X",
#endif
		"XC",
		"MAGICK",
		"GRANITE",
		"LOGO",
		"NETSCAPE",
		"ROSE"
	};

	elements = sizeof (virtual_fmt) / sizeof (virtual_fmt [0]);

	for (i = 0; i < elements; i++) {
		if (strcasecmp(format, virtual_fmt[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

static
zend_bool php_imagick_is_url(const char *filename TSRMLS_DC)
{
	const char *path_for_open;

	if (php_stream_locate_url_wrapper(filename, &path_for_open, STREAM_LOCATE_WRAPPERS_ONLY TSRMLS_CC)) {
		return 1;
	}
	return 0;
}

zend_bool php_imagick_file_init(struct php_imagick_file_t *file, const char *filename, size_t filename_len TSRMLS_DC)
{
	char magick_path[MaxTextExtent], head_path[MaxTextExtent], tail_path[MaxTextExtent], buffer[MaxTextExtent];

	if (!filename_len) {
		return 0;
	}

	/* Undefined for now */
	file->type = ImagickUndefinedType;

	if (filename_len >= MaxTextExtent) {
		return 0;
	}

	/* Take a copy of the original string */
	strlcpy(file->filename, filename, MaxTextExtent);
	file->filename_len = filename_len;

	/* Break the path into pieces */
	memset(magick_path, 0, MaxTextExtent);
	GetPathComponent(file->filename, MagickPath, magick_path);

	/* The path has a format identifier, check for url and virtual format */
	if (strlen(magick_path) > 0) {
		/* Virtual format? */
		if (php_imagick_is_virtual_format(magick_path)) {
			file->type          = ImagickVirtualFormat;
			file->absolute_path = estrdup("");
			return 1;
		}
		/* Is it an url? */
		else if (php_imagick_is_url(filename TSRMLS_CC)) {
			file->type          = ImagickUri;
			file->absolute_path = estrdup("");
			return 1;
		}
	}

	/* This is a normal file path */
	file->type = ImagickFile;

	memset(head_path, 0, MaxTextExtent);
	memset(tail_path, 0, MaxTextExtent);

	GetPathComponent(file->filename, HeadPath, head_path);
	GetPathComponent(file->filename, TailPath, tail_path);

	(void) snprintf(buffer, MaxTextExtent, "%s/%s", head_path, tail_path);

	/* The full path to the file */
	file->absolute_path = expand_filepath(buffer, NULL TSRMLS_CC);

	/* Failed to resolve absolute path */
	if (!file->absolute_path) {
		file->absolute_path = estrdup("");
	}
	return 1;
}

void php_imagick_file_deinit(struct php_imagick_file_t *file)
{
	if (file->absolute_path) {
		efree(file->absolute_path);
		file->absolute_path = NULL;
	}
}

static
int php_imagick_read_image_using_imagemagick(php_imagick_object *intern, struct php_imagick_file_t *file, ImagickOperationType type TSRMLS_DC)
{
	if (type == ImagickReadImage) {
		if (MagickReadImage(intern->magick_wand, file->filename) == MagickFalse) {
			struct stat st;
			/* Resolved to a filename. Check that it's not a dir */
			if (php_sys_stat(file->absolute_path, &st) == 0 && S_ISDIR(st.st_mode)) {
				return IMAGICK_RW_PATH_IS_DIR;
			}
			return IMAGICK_RW_UNDERLYING_LIBRARY;
		}
	} else if (type == ImagickPingImage){
		if (MagickPingImage(intern->magick_wand, file->filename) == MagickFalse) {
			return IMAGICK_RW_UNDERLYING_LIBRARY;
		}
	} else {
		return IMAGICK_RW_UNDERLYING_LIBRARY;
	}

	MagickSetImageFilename(intern->magick_wand, file->absolute_path);
	MagickSetLastIterator(intern->magick_wand);
	return IMAGICK_RW_OK;
}

static
int php_imagick_read_image_using_php_streams(php_imagick_object *intern, struct php_imagick_file_t *file, ImagickOperationType type TSRMLS_DC)
{
	php_stream *stream;
	MagickBooleanType status;
	FILE *fp;
	IMAGICK_INIT_ERROR_HANDLING;
	IMAGICK_SET_ERROR_HANDLING_THROW;

#ifdef ZEND_ENGINE_3
	stream = php_stream_open_wrapper(file->filename, "rb", (IGNORE_PATH) & ~REPORT_ERRORS, NULL);
#else
	stream = php_stream_open_wrapper(file->filename, "rb", (ENFORCE_SAFE_MODE|IGNORE_PATH) & ~REPORT_ERRORS, NULL);
#endif

	if (!stream) {
		IMAGICK_RESTORE_ERROR_HANDLING;
		return IMAGICK_RW_UNDERLYING_LIBRARY;
	}

	if (php_stream_can_cast(stream, PHP_STREAM_AS_STDIO|PHP_STREAM_CAST_INTERNAL) == FAILURE ||
		php_stream_cast(stream, PHP_STREAM_AS_STDIO|PHP_STREAM_CAST_INTERNAL, (void*)&fp, 0) == FAILURE) {

		php_stream_close(stream);
		IMAGICK_RESTORE_ERROR_HANDLING;
		return IMAGICK_RW_UNDERLYING_LIBRARY;
	}

	IMAGICK_RESTORE_ERROR_HANDLING;

	if (type == ImagickReadImage) {
		status = MagickReadImageFile(intern->magick_wand, fp);
	} else if (type == ImagickPingImage){
		status = MagickPingImageFile(intern->magick_wand, fp);
	} else {
		php_stream_close(stream);
		return IMAGICK_RW_UNDERLYING_LIBRARY;
	}

	if (status == MagickFalse) {
		php_stream_close(stream);
		return IMAGICK_RW_UNDERLYING_LIBRARY;
	}

	MagickSetImageFilename(intern->magick_wand, file->absolute_path);
	php_stream_close(stream);

	if (status == MagickFalse) {
		return IMAGICK_RW_UNDERLYING_LIBRARY;
	}

	MagickSetLastIterator(intern->magick_wand);
	return IMAGICK_RW_OK;
}

int php_imagick_safe_mode_check(const char *filename TSRMLS_DC)
{
#if defined(CHECKUID_CHECK_FILE_AND_DIR)
	if (PG(safe_mode) && (!php_checkuid_ex(filename, NULL, CHECKUID_CHECK_FILE_AND_DIR, CHECKUID_NO_ERRORS))) {
		return IMAGICK_RW_SAFE_MODE_ERROR;
	}
#endif
	if (PG(open_basedir) && php_check_open_basedir_ex(filename, 0 TSRMLS_CC)) {
		return IMAGICK_RW_OPEN_BASEDIR_ERROR;
	}

	return IMAGICK_RW_OK;
}

php_imagick_rw_result_t php_imagick_read_file(php_imagick_object *intern, struct php_imagick_file_t *file, ImagickOperationType type TSRMLS_DC)
{
	php_imagick_rw_result_t rc;

	if (file->type == ImagickFile) {
		rc = php_imagick_safe_mode_check(file->absolute_path TSRMLS_CC);

		if (rc != IMAGICK_RW_OK) {
			return rc;
		}
	}

	if (file->type == ImagickUri) {
		return php_imagick_read_image_using_php_streams(intern, file, type TSRMLS_CC);
	} else {
		return php_imagick_read_image_using_imagemagick(intern, file, type TSRMLS_CC);
	}
}

php_imagick_rw_result_t php_imagick_write_file(php_imagick_object *intern, struct php_imagick_file_t *file, ImagickOperationType type, zend_bool adjoin TSRMLS_DC)
{
	php_imagick_rw_result_t rc;
	MagickBooleanType status = MagickFalse;

	if (file->type == ImagickFile) {
		rc = php_imagick_safe_mode_check(file->absolute_path TSRMLS_CC);
		if (rc != IMAGICK_RW_OK) {
			return rc;
		}
	}
	if (type == ImagickWriteImage) {
		status = MagickWriteImage(intern->magick_wand, file->filename);
	} else if (type == ImagickWriteImages) {
		status = MagickWriteImages(intern->magick_wand, file->filename, adjoin);
	}

	/* Write succeded ? */
	if (status == MagickFalse) {
		return IMAGICK_RW_UNDERLYING_LIBRARY;
	}
	/* All went well it seems */
	return IMAGICK_RW_OK;
}

zend_bool php_imagick_stream_handler(php_imagick_object *intern, php_stream *stream, ImagickOperationType type TSRMLS_DC)
{
	FILE *fp;
	MagickBooleanType status = MagickFalse;

	IMAGICK_INIT_ERROR_HANDLING;
	IMAGICK_SET_ERROR_HANDLING_THROW;

	if (php_stream_can_cast(stream, PHP_STREAM_AS_STDIO | PHP_STREAM_CAST_INTERNAL) == FAILURE ||
		php_stream_cast(stream, PHP_STREAM_AS_STDIO | PHP_STREAM_CAST_INTERNAL, (void*)&fp, 0) == FAILURE) {
		IMAGICK_RESTORE_ERROR_HANDLING;
		return 0;
	}

	IMAGICK_RESTORE_ERROR_HANDLING;

	/* php_stream_cast returns warning on some streams but still does not return FAILURE */
	if (EG(exception)) {
		return 0;
	}

	switch (type) {
		case ImagickWriteImageFile:
			status = MagickWriteImageFile(intern->magick_wand, fp);
		break;

		case ImagickWriteImagesFile:
			status = MagickWriteImagesFile(intern->magick_wand, fp);
		break;

		case ImagickReadImageFile:
			status = MagickReadImageFile(intern->magick_wand, fp);
		break;

		case ImagickPingImageFile:
			status = MagickPingImageFile(intern->magick_wand, fp);
		break;

		default:
			return 0;
		break;
	}
	if (status == MagickFalse) {
		return 0;
	}
	return 1;
}

