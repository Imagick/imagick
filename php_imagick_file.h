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
#ifndef PHP_IMAGICK_FILE_H
# define PHP_IMAGICK_FILE_H

#include "php_imagick.h"
#include "php_imagick_defs.h"

typedef enum {
	ImagickUndefinedType, /* Don't know */
	ImagickFile,          /* Use ImageMagick to read the file */
	ImagickUri,           /* Use PHP streams to read the file */
	ImagickVirtualFormat, /* The file is a virtual, use ImageMagick */
} ImagickFileType;

struct php_imagick_file_t {
	
	/* The file type */
	ImagickFileType type;

	/* Absolute path to the file, emalloced */
	char *absolute_path;
	size_t absolute_path_len;

	/* Original filename */
	char filename[MaxTextExtent];
	size_t filename_len;
};

typedef enum {
	ImagickUndefinedOperation,
	ImagickReadImage,
	ImagickPingImage,
	ImagickWriteImage,
	ImagickWriteImages,
	
	ImagickWriteImageFile,
	ImagickWriteImagesFile,
	
	ImagickReadImageFile,
	ImagickPingImageFile,
} ImagickOperationType;


zend_bool php_imagick_file_init(struct php_imagick_file_t *file, const char *filename, size_t filename_len TSRMLS_DC);

void php_imagick_file_deinit(struct php_imagick_file_t *file);

int php_imagick_safe_mode_check(const char *filename TSRMLS_DC);

/* Read operations */
int php_imagick_read_file(php_imagick_object *intern, struct php_imagick_file_t *file, ImagickOperationType type TSRMLS_DC);

/* Write operations */
int php_imagick_write_file(php_imagick_object *intern, struct php_imagick_file_t *file, ImagickOperationType type, zend_bool adjoin TSRMLS_DC);

/* Handle streams */
int php_imagick_stream_handler(php_imagick_object *intern, php_stream *stream, ImagickOperationType type TSRMLS_DC);

#endif /* PHP_IMAGICK_FILE_H */