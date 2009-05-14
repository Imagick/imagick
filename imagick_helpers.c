/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Imagick											  |
   +----------------------------------------------------------------------+
   | Copyright (c) 2006-2009 Mikko Koppanen, Scott MacVicar				  |
   | Imagemagick (c) ImageMagick Studio LLC								  |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,	  |
   | that is bundled with this package in the file LICENSE, and is		  |
   | available through the world-wide-web at the following url:			  |
   | http://www.php.net/license/3_01.txt								  |
   | If you did not receive a copy of the PHP license and are unable to	  |
   | obtain it through the world-wide-web, please send a note to		  |
   | license@php.net so we can mail you a copy immediately.				  |
   +----------------------------------------------------------------------+
   | Author: Mikko Kopppanen <mkoppanen@php.net>						  |
   |		 Scott MacVicar <scottmac@php.net>							  |
   +----------------------------------------------------------------------+
*/

#include "php_imagick.h"
#include "php_imagick_defs.h"
#include "php_imagick_macros.h"

MagickBooleanType php_imagick_progress_monitor(const char *text, const MagickOffsetType offset, const MagickSizeType span, void *client_data)
{
	FILE *fp;
	php_imagick_object *intern = (php_imagick_object *)client_data;
	
	if (!intern) {
		return MagickFalse;
	}
	
	if (!intern->progress_monitor_name) {
		return MagickFalse;
	}

	fp = fopen(intern->progress_monitor_name, "a+");
	
	if (!fp) {
		return MagickFalse;
	}

	fprintf(fp, "text: %s, offset: %ld, span: %ld\n", text, offset, span);
	fclose(fp);
	return MagickTrue;	
}

/*
	type 1 = MagickWriteImageFile
	type 2 = MagickWriteImagesFile
	
	type 3 = MagickReadImageFile
	type 4 = MagickPingImageFile

	return values:
		0 - no error
		1 - error and exception has been thrown
		2 - error but exception needs to be thrown
*/
int php_imagick_stream_handler(php_imagick_object *intern, php_stream *stream, char *filename, int type TSRMLS_DC)
{
#if ZEND_MODULE_API_NO > 20060613 
	zend_error_handling error_handling;
#endif
	FILE *fp;
	MagickBooleanType status = MagickFalse;

#if ZEND_MODULE_API_NO > 20060613 
	zend_replace_error_handling(EH_THROW, php_imagick_exception_class_entry, &error_handling TSRMLS_CC);
#else
	php_set_error_handling(EH_THROW, php_imagick_exception_class_entry TSRMLS_CC);
#endif	

	if (php_stream_can_cast(stream, PHP_STREAM_AS_STDIO | PHP_STREAM_CAST_INTERNAL) == FAILURE) {
		goto return_on_error;
	}

	if (php_stream_cast(stream, PHP_STREAM_AS_STDIO | PHP_STREAM_CAST_INTERNAL, (void*)&fp, 0) == FAILURE) {
		goto return_on_error;
	}
	
#if ZEND_MODULE_API_NO > 20060613 
	zend_restore_error_handling(&error_handling TSRMLS_CC);
#else
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
#endif	

	/* php_stream_cast returns warning on some streams but still does not return FAILURE */
	if (EG(exception)) {
		return 1;
	} else {
		return 2;
	}

	if (type == IMAGICK_WRITE_IMAGE_FILE) {
		status = MagickWriteImageFile(intern->magick_wand, fp);
	} else if (type == IMAGICK_WRITE_IMAGES_FILE) {
		status = MagickWriteImagesFile(intern->magick_wand, fp);
	} else if (type == IMAGICK_READ_IMAGE_FILE) {		
		status = MagickReadImageFile(intern->magick_wand, fp);
	} else if (type == IMAGICK_PING_IMAGE_FILE) {
		status = MagickPingImageFile(intern->magick_wand, fp);
	} 

	if (status == MagickFalse) {
		return 2;
	}
	
	if (filename) {
		MagickSetImageFilename(intern->magick_wand, filename);
		IMAGICK_CORRECT_ITERATOR_POSITION(intern);
	}	
	
	return 0;
	
return_on_error:
#if ZEND_MODULE_API_NO > 20060613 
	zend_restore_error_handling(&error_handling TSRMLS_CC);
#else
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
#endif
	if (EG(exception)) {
		return 1;
	} else {
		return 2;
	}
}

zend_bool php_imagick_validate_map(const char *map TSRMLS_DC)
{
	zend_bool match;
	char *p = map;
	char allow_map[] = { 'R', 'G', 'B', 
						 'A', 'O', 'C', 
						 'Y', 'M', 'K', 
						 'I', 'P' };

	while (*p != '\0') {
		char *it = allow_map;
		match = 0;
		while(*it != '\0') {
			if (*(it++) == *p) {
				match = 1;
			}
		}
		if (!match) {
			return 0;
		}
		*(p++);
	}
	return 1;
}

int count_occurences_of(char needle, char *haystack TSRMLS_DC)
{
	int occurances = 0;

	if (haystack == (char *)NULL) {
		return 0;
	}

	while (*haystack != '\0') {
		if (*(haystack++) == needle) {
			occurances++;
		}
	}
	return occurances;
}

void add_assoc_string_helper(zval *retvalue, char *name, char *key, char *hash_value TSRMLS_DC)
{
	char *pch;
	char *trimmed;
	zval *array;
	int width, height;
	double x, y;

	if (strncmp(hash_value, name, strlen(name)) == 0) {
		
		if (strcmp("geometry", key) == 0) {

			MAKE_STD_ZVAL(array);
			array_init(array);
			sscanf(hash_value, "%*s %d%*c%d", &width, &height);
			add_assoc_long(array, "width", width);
			add_assoc_long(array, "height", height);
			add_assoc_zval(retvalue, key, array);
		
		} else if (strcmp("resolution" , key) == 0) {

			MAKE_STD_ZVAL(array);
			array_init(array);

			sscanf(hash_value, "%*s %lf%*c%lf", &x, &y);
			add_assoc_double(array, "x", x);
			add_assoc_double(array, "y", y);
			add_assoc_zval(retvalue, key, array);
		
		} else {

			pch = strchr(hash_value, ':');
			pch = strchr(pch + 1, ' ');
			trimmed = php_trim(pch, strlen(pch), (char *)NULL, 0, NULL, 3 TSRMLS_CC);
			add_assoc_string(retvalue, key, trimmed, 1);
			efree(trimmed);
		}
	}
}

double *get_double_array_from_zval(zval *param_array, long *num_elements TSRMLS_DC)
{
	zval **ppzval;
	HashTable *ht;
	double *double_array;
	long elements, i;

	*num_elements = 0;
	elements = zend_hash_num_elements(Z_ARRVAL_P(param_array));

	if (elements == 0) {
		double_array = (double *)NULL;
		return double_array;
	}

	double_array = (double *)emalloc(sizeof(double) * elements);
	ht = Z_ARRVAL_P(param_array);

	zend_hash_internal_pointer_reset(ht);

	for (i = 0 ; i < elements ; i++) {

		if (zend_hash_get_current_data(ht, (void**)&ppzval) == FAILURE) {
			efree(double_array);
			double_array = (double *)NULL;
			return double_array;
		}

		if(Z_TYPE_PP(ppzval) == IS_LONG) {
			double_array[i] = (double)Z_LVAL_PP(ppzval);
		} else if (Z_TYPE_PP(ppzval) == IS_DOUBLE) {
			double_array[i] = Z_DVAL_PP(ppzval);
		} else {
			efree(double_array);
			double_array = (double *)NULL;
			return double_array;
		}

		zend_hash_move_forward(ht);
	}
	*num_elements = elements;
	return double_array;
}

long *get_long_array_from_zval(zval *param_array, long *num_elements TSRMLS_DC)
{
	zval **ppzval;
	HashTable *ht;
	long *long_array = NULL;
	long elements, i;

	*num_elements = 0;
	elements = zend_hash_num_elements(Z_ARRVAL_P(param_array));

	if (elements == 0) {
		return long_array;
	}

	long_array = emalloc(sizeof(long) * elements);
	ht = Z_ARRVAL_P(param_array);

	zend_hash_internal_pointer_reset(ht);

	for (i = 0 ; i < elements ; i++) {
		if (zend_hash_get_current_data(ht, (void**)&ppzval) == FAILURE) {
			efree(long_array);
			long_array = NULL;
			return long_array;
		}
		
		if ((Z_TYPE_PP(ppzval) == IS_LONG) || (Z_TYPE_PP(ppzval) == IS_DOUBLE)) {
			long_array[i] = Z_LVAL_PP(ppzval);
		} else {
			efree(long_array);
			long_array = NULL;
			return long_array;
		}
		zend_hash_move_forward(ht);
	}
	*num_elements = elements;
	return long_array;
}

unsigned char *get_char_array_from_zval(zval *param_array, long *num_elements TSRMLS_DC)
{
	zval **ppzval;
	HashTable *ht;
	unsigned char *char_array = NULL;
	long elements, i;

	*num_elements = 0;
	elements = zend_hash_num_elements(Z_ARRVAL_P(param_array));

	if (elements == 0) {
		return char_array;
	}

	char_array = emalloc(sizeof(char) * elements);
	ht = Z_ARRVAL_P(param_array);

	zend_hash_internal_pointer_reset(ht);

	for (i = 0 ; i < elements ; i++) {

		if (zend_hash_get_current_data(ht, (void**)&ppzval) == FAILURE) {
			efree(char_array);
			char_array = NULL;
			return char_array;
		}
		
		if ((Z_TYPE_PP(ppzval) == IS_LONG) || (Z_TYPE_PP(ppzval) == IS_DOUBLE)) {
			char_array[i] = (unsigned char)Z_LVAL_PP(ppzval);
		} else {
			efree(char_array);
			char_array = NULL;
			return char_array;
		}
		zend_hash_move_forward(ht);
	}
	*num_elements = elements;
	return char_array;
}

int check_configured_font(char *font, int font_len TSRMLS_DC)
{
	int retval = 0;
	char **fonts;
	unsigned long num_fonts = 0, i = 0;

	/* Check that user is only able to set a proper font */
	fonts = (char **) MagickQueryFonts("*", &num_fonts);

	for(i = 0 ; i < num_fonts ; i++) {
		/* Let's see if the font is among configured fonts */
		if (strncasecmp(fonts[i], font, font_len) == 0) {
			retval = 1;
			break;
		}
	}

	IMAGICK_FREE_MEMORY(char **, fonts);
	return retval;
}

int check_write_access(char *absolute TSRMLS_DC)
{
	/* Check if file exists */
	if (VCWD_ACCESS(absolute, F_OK)) {

		if (!VCWD_ACCESS(absolute, W_OK)) {

			efree(absolute);
			return IMAGICK_READ_WRITE_PERMISSION_DENIED;

		} else {

			/* File is not there. Check that dir exists and that its writable */
			char path[MAXPATHLEN];
			size_t path_len;
			memset(path, '\0', MAXPATHLEN);
			memcpy(path, absolute, strlen(absolute));
			path_len = php_dirname(path, strlen(absolute));

			/* Path does not exist */
			if (!VCWD_ACCESS(path, F_OK)) {
				zval *ret;
				MAKE_STD_ZVAL(ret);
				
				/* stat to make sure the path is actually a directory */
				php_stat(path, path_len, FS_IS_DIR, ret TSRMLS_CC);
	
				/* It is not a dir */
				if (Z_TYPE_P(ret) == IS_BOOL && Z_BVAL_P(ret) == 0) {
					FREE_ZVAL(ret);
					return IMAGICK_READ_WRITE_PATH_DOES_NOT_EXIST;
				}
				FREE_ZVAL(ret);
				
				if (VCWD_ACCESS(path, W_OK)) {
					return IMAGICK_READ_WRITE_PERMISSION_DENIED;
				}
					
			} else {
				return IMAGICK_READ_WRITE_PATH_DOES_NOT_EXIST;
			}
								
			/* Can't write the file */
			if (VCWD_ACCESS(path, W_OK)) {
				return IMAGICK_READ_WRITE_PERMISSION_DENIED;
			}
		}
	}
	return IMAGICK_READ_WRITE_NO_ERROR;
}

zend_bool crop_thumbnail_image(MagickWand *magick_wand, long desired_width, long desired_height TSRMLS_DC)
{
	double ratio;
	long crop_x = 0, crop_y = 0, image_width, image_height;
	
	long orig_width = MagickGetImageWidth(magick_wand);
	long orig_height = MagickGetImageHeight(magick_wand);
	
	/* Already at the size, just strip profiles */
	if ((orig_width == desired_width) && (orig_height == desired_height)) {
		if (!MagickStripImage(magick_wand)) {
			return 0;
		}
		return 1;
	}

	if (((double)orig_width - (double)desired_width) > ((double)orig_height - (double)desired_height)) {
		ratio		 = (double)orig_height / (double)desired_height;
		image_width	 = (double)orig_width / (double)ratio;
		image_height = desired_height;
		crop_x		 = ((double)image_width - (double)desired_width) / 2;
	} else {
		ratio		 = (double)orig_width / (double)desired_width;
		image_height = (double)orig_height / (double)ratio;
		image_width	 = desired_width;
		crop_y		 = ((double)image_height - (double)desired_height) / 2;
	}
	
	if (desired_width != orig_width && desired_height != orig_height) {
		if (!MagickThumbnailImage(magick_wand, image_width, image_height)) {
			return 0;
		}
	}

	if (!MagickCropImage(magick_wand, desired_width, desired_height, crop_x, crop_y)) {
		return 0;
	}
	
	MagickSetImagePage(magick_wand, desired_width, desired_height, 0, 0);
	return 1;
}

void deallocate_wands(MagickWand *magick, DrawingWand *draw, PixelWand *pixel TSRMLS_DC)
{
	if (magick != (MagickWand *)NULL) {
		magick = (MagickWand *)DestroyMagickWand(magick);
	}

	if (draw != (DrawingWand *)NULL) {
		draw = (DrawingWand *)DestroyDrawingWand(draw);
	}

	if (pixel != (PixelWand *)NULL) {
		pixel = (PixelWand *)DestroyPixelWand(pixel);
	}
}

void *get_pointinfo_array(zval *coordinate_array, int *num_elements TSRMLS_DC)
{
	PointInfo *coordinates;
	long elements, sub_elements, i;
	HashTable *coords;
	zval **ppzval, **ppz_x, **ppz_y;
	HashTable *sub_array;

	elements = zend_hash_num_elements(Z_ARRVAL_P(coordinate_array));

	if (elements < 1) {
		coordinates = (PointInfo *)NULL;
		*num_elements = 0;
		return coordinates;
	}

	*num_elements = elements;
	coordinates = (PointInfo *)emalloc(sizeof(PointInfo) * elements);

	coords = Z_ARRVAL_P(coordinate_array);
	zend_hash_internal_pointer_reset_ex(coords, (HashPosition *) 0);

	for (i = 0 ; i < elements ; i++) {

		/* Get the sub array */
		if (zend_hash_get_current_data(coords, (void**)&ppzval) == FAILURE) {
			coordinates = (PointInfo *)NULL;
			efree(coordinates);
			*num_elements = 0;
			return coordinates;
		}

		/* If its something than array lets error here */
		if(Z_TYPE_PP(ppzval) != IS_ARRAY) {
			coordinates = (PointInfo *)NULL;
			efree(coordinates);
			*num_elements = 0;
			return coordinates;
		}

		/* Subarray should have two elements. X and Y */
		sub_elements = zend_hash_num_elements(Z_ARRVAL_PP(ppzval));

		/* Exactly two elements */
		if (sub_elements != 2) {
			*num_elements = 0;
			efree(coordinates);
			coordinates = (PointInfo *)NULL;
			return coordinates;
		}

		/* Subarray values */
		sub_array = Z_ARRVAL_PP(ppzval);

		/* Get X */
		if (zend_hash_find(sub_array, "x", sizeof("x"), (void**)&ppz_x) == FAILURE) {
			efree(coordinates);
			coordinates = (PointInfo *)NULL;
			*num_elements = 0;
			return coordinates;
		}

		if(Z_TYPE_PP(ppz_x) != IS_DOUBLE && Z_TYPE_PP(ppz_x) != IS_LONG) {
			efree(coordinates);
			coordinates = (PointInfo *)NULL;
			*num_elements = 0;
			return coordinates;
		}

		/* Get Y */
		if (zend_hash_find(sub_array, "y", sizeof("y"), (void**)&ppz_y) == FAILURE) {
			efree(coordinates);
			coordinates = (PointInfo *)NULL;
			*num_elements = 0;
			return coordinates;
		}

		if(Z_TYPE_PP(ppz_y) != IS_DOUBLE && Z_TYPE_PP(ppz_y) != IS_LONG) {
			efree(coordinates);
			coordinates = (PointInfo *)NULL;
			*num_elements = 0;
			return coordinates;
		}

		/* Assign X and Y */

		if (Z_TYPE_PP(ppz_x) == IS_LONG) {
			coordinates[i].x = (double)Z_LVAL_PP(ppz_x);
		} else {
			coordinates[i].x = Z_DVAL_PP(ppz_x);
		}

		if (Z_TYPE_PP(ppz_y) == IS_LONG) {
			coordinates[i].y = (double)Z_LVAL_PP(ppz_y);
		} else {
			coordinates[i].y = Z_DVAL_PP(ppz_y);
		}
		zend_hash_move_forward(coords);
	}

	return coordinates;
}

#if MagickLibVersion <= 0x628
void count_pixeliterator_rows(php_imagickpixeliterator_object *internpix TSRMLS_DC)
{
	long rows = 0, tmp;
	PixelWand **row;
	(void) PixelResetIterator(internpix->pixel_iterator);

	while ((row = (PixelWand **)PixelGetNextIteratorRow(internpix->pixel_iterator, &tmp))) {
		if (row == (PixelWand **)NULL) {
			break;
		}
		rows++;
	}
	internpix->rows = rows;
}
#endif

char *get_pseudo_filename(char *pseudo_string TSRMLS_DC)
{
	char *filename = NULL;
	char *ptr = strchr(pseudo_string, ':');
	
	/* No colon */
	if(ptr == NULL) {
		return NULL;
	} else {
		++ptr; /* Move one position, removing colon from filename */
		filename = estrdup(ptr);
	}
	return filename;
}

/* type 1 = writeimage, type 2 = writeimages */
int write_image_from_filename(php_imagick_object *intern, char *filename, zend_bool adjoin, int type TSRMLS_DC)
{
	int pos = 0, occurances = 0, add_format = 0, error = IMAGICK_READ_WRITE_NO_ERROR;
	char *buffer, *filepath, c;
	MagickBooleanType status;

	occurances = count_occurences_of(':', filename TSRMLS_CC);

#if defined(PHP_WIN32)
	/* windows can have things like png:C:\test.png */
	if (occurances > 0) {
		occurances -= 1;
	}
#endif

	if (strlen(filename) > MAXPATHLEN) {
		return IMAGICK_READ_WRITE_FILENAME_TOO_LONG;
	}

	/* Assume format in the filename. For example: png:hello.jpg */
	if (occurances > 0) {
		buffer = filename;
		
		/* Strip everything before the first : */
		while (((c = *(buffer++)) != '\0') && c != ':') { pos++; };
		
		/* This can happen with filename like "png:" */
		if (buffer == '\0' || strlen(buffer) == 0) {
			return IMAGICK_READ_WRITE_UNDERLYING_LIBRARY;
		}
		
		/* Absolute path to the file */
		filepath = expand_filepath(buffer, NULL TSRMLS_CC);
	
		/* Indicate that the final format string will need the format */
		add_format = 1;
	} else {
		filepath = expand_filepath(filename, NULL TSRMLS_CC);
		add_format = 0;
	}
	
	/* Check safe mode and open basedir */
	IMAGICK_SAFE_MODE_CHECK(filepath, error);
	if (error != IMAGICK_READ_WRITE_NO_ERROR) {
		efree(filepath);
		return error;
	}

	/* Bypass a bug in imagemagick. write failure causes a segfault*/
	error = check_write_access(filepath TSRMLS_CC);
	if (error != IMAGICK_READ_WRITE_NO_ERROR) {
		efree(filepath);
		return error;
	}

	/* Add the format to the string ? */
	if (add_format) {
		char *format = NULL, *tmp_filepath = estrdup(filepath);
		efree(filepath);
		filepath = NULL;
		
		format = emalloc(pos+1);
		format[0] = '\0';
		strncat(format, filename, pos);
		
		spprintf(&filepath, 0, "%s:%s", format, tmp_filepath);
		efree(tmp_filepath);
		efree(format);
	}

	/* Write image or write images */
	if (type == 1) {
		status = MagickWriteImage(intern->magick_wand, filepath);
	} else {
		status = MagickWriteImages(intern->magick_wand, filepath, adjoin);
	}
	efree(filepath);

	/* Write succeded ? */
	if (status == MagickFalse) {
		return IMAGICK_READ_WRITE_UNDERLYING_LIBRARY;
	}
	
	/* All went well it seems */
	return IMAGICK_READ_WRITE_NO_ERROR;
}


void initialize_imagick_constants()
{
	TSRMLS_FETCH();

	/* Constants defined in php_imagick.h */
	IMAGICK_REGISTER_CONST_LONG("COLOR_BLACK", IMAGICKCOLORBLACK);
	IMAGICK_REGISTER_CONST_LONG("COLOR_BLUE", IMAGICKCOLORBLUE);
	IMAGICK_REGISTER_CONST_LONG("COLOR_CYAN", IMAGICKCOLORCYAN);
	IMAGICK_REGISTER_CONST_LONG("COLOR_GREEN", IMAGICKCOLORGREEN);
	IMAGICK_REGISTER_CONST_LONG("COLOR_RED", IMAGICKCOLORRED);
	IMAGICK_REGISTER_CONST_LONG("COLOR_YELLOW", IMAGICKCOLORYELLOW);
	IMAGICK_REGISTER_CONST_LONG("COLOR_MAGENTA", IMAGICKCOLORMAGENTA);
	IMAGICK_REGISTER_CONST_LONG("COLOR_OPACITY", IMAGICKCOLOROPACITY);
	IMAGICK_REGISTER_CONST_LONG("COLOR_ALPHA", IMAGICKCOLORALPHA);
	IMAGICK_REGISTER_CONST_LONG("COLOR_FUZZ", IMAGICKCOLORFUZZ);

	/* Returning the version as a constant string */
	IMAGICK_REGISTER_CONST_LONG("IMAGICK_EXTNUM", PHP_IMAGICK_EXTNUM);
	IMAGICK_REGISTER_CONST_STRING("IMAGICK_EXTVER", PHP_IMAGICK_VERSION);

	/* ImageMagick defined constants */
#if defined(OverCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DEFAULT", OverCompositeOp);
#endif
#if defined(UndefinedCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_UNDEFINED", UndefinedCompositeOp);
#endif
#if defined(NoCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_NO", NoCompositeOp);
#endif
#if defined(AddCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_ADD", AddCompositeOp);
#endif
#if defined(AtopCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_ATOP", AtopCompositeOp);
#endif
#if defined(BlendCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_BLEND", BlendCompositeOp);
#endif
#if defined(BumpmapCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_BUMPMAP", BumpmapCompositeOp);
#endif
#if defined(ClearCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_CLEAR", ClearCompositeOp);
#endif
#if defined(ColorBurnCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COLORBURN", ColorBurnCompositeOp);
#endif
#if defined(ColorDodgeCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COLORDODGE", ColorDodgeCompositeOp);
#endif
#if defined(ColorizeCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COLORIZE", ColorizeCompositeOp);
#endif
#if defined(CopyBlackCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYBLACK", CopyBlackCompositeOp);
#endif
#if defined(CopyBlueCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYBLUE", CopyBlueCompositeOp);
#endif
#if defined(CopyCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPY", CopyCompositeOp);
#endif
#if defined(CopyCyanCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYCYAN", CopyCyanCompositeOp);
#endif
#if defined(CopyGreenCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYGREEN", CopyGreenCompositeOp);
#endif
#if defined(CopyMagentaCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYMAGENTA", CopyMagentaCompositeOp);
#endif
#if defined(CopyOpacityCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYOPACITY", CopyOpacityCompositeOp);
#endif
#if defined(CopyRedCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYRED", CopyRedCompositeOp);
#endif
#if defined(CopyYellowCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYYELLOW", CopyYellowCompositeOp);
#endif
#if defined(DarkenCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DARKEN", DarkenCompositeOp);
#endif
#if defined(DstAtopCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DSTATOP", DstAtopCompositeOp);
#endif
#if defined(DstCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DST", DstCompositeOp);
#endif
#if defined(DstInCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DSTIN", DstInCompositeOp);
#endif
#if defined(DstOutCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DSTOUT", DstOutCompositeOp);
#endif
#if defined(DstOverCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DSTOVER", DstOverCompositeOp);
#endif
#if defined(DifferenceCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DIFFERENCE", DifferenceCompositeOp);
#endif
#if defined(DisplaceCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DISPLACE", DisplaceCompositeOp);
#endif
#if defined(DissolveCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DISSOLVE", DissolveCompositeOp);
#endif
#if defined(ExclusionCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_EXCLUSION", ExclusionCompositeOp);
#endif
#if defined(HardLightCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_HARDLIGHT", HardLightCompositeOp);
#endif
#if defined(HueCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_HUE", HueCompositeOp);
#endif
#if defined(InCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_IN", InCompositeOp);
#endif
#if defined(LightenCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_LIGHTEN", LightenCompositeOp);
#endif
#if defined(LuminizeCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_LUMINIZE", LuminizeCompositeOp);
#endif
#if defined(MinusCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_MINUS", MinusCompositeOp);
#endif
#if defined(ModulateCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_MODULATE", ModulateCompositeOp);
#endif
#if defined(MultiplyCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_MULTIPLY", MultiplyCompositeOp);
#endif
#if defined(OutCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_OUT", OutCompositeOp);
#endif
#if defined(OverCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_OVER", OverCompositeOp);
#endif
#if defined(OverlayCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_OVERLAY", OverlayCompositeOp);
#endif
#if defined(PlusCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_PLUS", PlusCompositeOp);
#endif
#if defined(ReplaceCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_REPLACE", ReplaceCompositeOp);
#endif
#if defined(SaturateCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SATURATE", SaturateCompositeOp);
#endif
#if defined(ScreenCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SCREEN", ScreenCompositeOp);
#endif
#if defined(SoftLightCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SOFTLIGHT", SoftLightCompositeOp);
#endif
#if defined(SrcAtopCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SRCATOP", SrcAtopCompositeOp);
#endif
#if defined(SrcCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SRC", SrcCompositeOp);
#endif
#if defined(SrcInCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SRCIN", SrcInCompositeOp);
#endif
#if defined(SrcOutCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SRCOUT", SrcOutCompositeOp);
#endif
#if defined(SrcOverCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SRCOVER", SrcOverCompositeOp);
#endif
#if defined(SubtractCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SUBTRACT", SubtractCompositeOp);
#endif
#if defined(ThresholdCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_THRESHOLD", ThresholdCompositeOp);
#endif
#if defined(XorCompositeOp)
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_XOR", XorCompositeOp);
#endif
#if defined(FrameMode)
	IMAGICK_REGISTER_CONST_LONG("MONTAGEMODE_FRAME", FrameMode);
#endif
#if defined(UnframeMode)
	IMAGICK_REGISTER_CONST_LONG("MONTAGEMODE_UNFRAME", UnframeMode);
#endif
#if defined(ConcatenateMode)
	IMAGICK_REGISTER_CONST_LONG("MONTAGEMODE_CONCATENATE", ConcatenateMode);
#endif
#if defined(NormalStyle)
	IMAGICK_REGISTER_CONST_LONG("STYLE_NORMAL", NormalStyle );
#endif
#if defined(ItalicStyle)
	IMAGICK_REGISTER_CONST_LONG("STYLE_ITALIC", ItalicStyle );
#endif
#if defined(ObliqueStyle)
	IMAGICK_REGISTER_CONST_LONG("STYLE_OBLIQUE", ObliqueStyle );
#endif
#if defined(AnyStyle)
	IMAGICK_REGISTER_CONST_LONG("STYLE_ANY", AnyStyle );
#endif
#if defined(UndefinedFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_UNDEFINED", UndefinedFilter );
#endif
#if defined(PointFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_POINT", PointFilter );
#endif
#if defined(BoxFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_BOX", BoxFilter );
#endif
#if defined(TriangleFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_TRIANGLE", TriangleFilter );
#endif
#if defined(HermiteFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_HERMITE", HermiteFilter );
#endif
#if defined(HanningFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_HANNING", HanningFilter );
#endif
#if defined(HammingFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_HAMMING", HammingFilter );
#endif
#if defined(BlackmanFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_BLACKMAN", BlackmanFilter );
#endif
#if defined(GaussianFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_GAUSSIAN", GaussianFilter );
#endif
#if defined(QuadraticFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_QUADRATIC", QuadraticFilter );
#endif
#if defined(CubicFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_CUBIC", CubicFilter );
#endif
#if defined(CatromFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_CATROM", CatromFilter );
#endif
#if defined(MitchellFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_MITCHELL", MitchellFilter );
#endif
#if defined(LanczosFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_LANCZOS", LanczosFilter );
#endif
#if defined(BesselFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_BESSEL", BesselFilter );
#endif
#if defined(SincFilter)
	IMAGICK_REGISTER_CONST_LONG("FILTER_SINC", SincFilter );
#endif
#if defined(UndefinedType)
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_UNDEFINED", UndefinedType );
#endif
#if defined(BilevelType)
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_BILEVEL", BilevelType );
#endif
#if defined(GrayscaleType)
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_GRAYSCALE", GrayscaleType );
#endif
#if defined(GrayscaleMatteType)
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_GRAYSCALEMATTE", GrayscaleMatteType );
#endif
#if defined(PaletteType)
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_PALETTE",	PaletteType );
#endif
#if defined(PaletteMatteType)
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_PALETTEMATTE", PaletteMatteType );
#endif
#if defined(TrueColorType)
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_TRUECOLOR", TrueColorType );
#endif
#if defined(TrueColorMatteType)
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_TRUECOLORMATTE", TrueColorMatteType );
#endif
#if defined(ColorSeparationType)
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_COLORSEPARATION", ColorSeparationType );
#endif
#if defined(ColorSeparationMatteType)
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_COLORSEPARATIONMATTE", ColorSeparationMatteType );
#endif
#if defined(OptimizeType)
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_OPTIMIZE", OptimizeType );
#endif
#if defined(UndefinedResolution)
	IMAGICK_REGISTER_CONST_LONG("RESOLUTION_UNDEFINED", UndefinedResolution );
#endif
#if defined(PixelsPerInchResolution)
	IMAGICK_REGISTER_CONST_LONG("RESOLUTION_PIXELSPERINCH", PixelsPerInchResolution );
#endif
#if defined(PixelsPerCentimeterResolution)
	IMAGICK_REGISTER_CONST_LONG("RESOLUTION_PIXELSPERCENTIMETER", PixelsPerCentimeterResolution);
#endif
#if defined(UndefinedCompression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_UNDEFINED", UndefinedCompression);
#endif
#if defined(NoCompression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_NO", NoCompression);
#endif
#if defined(BZipCompression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_BZIP", BZipCompression);
#endif
#if defined(FaxCompression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_FAX", FaxCompression);
#endif
#if defined(Group4Compression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_GROUP4", Group4Compression);
#endif
#if defined(JPEGCompression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_JPEG", JPEGCompression);
#endif
#if defined(JPEG2000Compression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_JPEG2000", JPEG2000Compression);
#endif
#if defined(LosslessJPEGCompression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_LOSSLESSJPEG", LosslessJPEGCompression);
#endif
#if defined(LZWCompression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_LZW", LZWCompression);
#endif
#if defined(RLECompression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_RLE", RLECompression);
#endif
#if defined(ZipCompression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_ZIP", ZipCompression);
#endif
#if defined(DXT1Compression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_DXT1", DXT1Compression);
#endif
#if defined(DXT3Compression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_DXT3", DXT3Compression);
#endif
#if defined(DXT5Compression)
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_DXT5", DXT5Compression);
#endif
#if defined(PointMethod)
	IMAGICK_REGISTER_CONST_LONG("PAINT_POINT", PointMethod);
#endif
#if defined(ReplaceMethod)
	IMAGICK_REGISTER_CONST_LONG("PAINT_REPLACE", ReplaceMethod);
#endif
#if defined(FloodfillMethod)
	IMAGICK_REGISTER_CONST_LONG("PAINT_FLOODFILL", FloodfillMethod);
#endif
#if defined(FillToBorderMethod)
	IMAGICK_REGISTER_CONST_LONG("PAINT_FILLTOBORDER", FillToBorderMethod);
#endif
#if defined(ResetMethod)
	IMAGICK_REGISTER_CONST_LONG("PAINT_RESET", ResetMethod);
#endif
#if defined(NorthWestGravity)
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_NORTHWEST", NorthWestGravity);
#endif
#if defined(NorthGravity)
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_NORTH", NorthGravity);
#endif
#if defined(NorthEastGravity)
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_NORTHEAST", NorthEastGravity);
#endif
#if defined(WestGravity)
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_WEST", WestGravity);
#endif
#if defined(CenterGravity)
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_CENTER", CenterGravity);
#endif
#if defined(EastGravity)
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_EAST", EastGravity);
#endif
#if defined(SouthWestGravity)
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_SOUTHWEST", SouthWestGravity);
#endif
#if defined(SouthGravity)
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_SOUTH", SouthGravity);
#endif
#if defined(SouthEastGravity)
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_SOUTHEAST", SouthEastGravity);
#endif
#if defined(NormalStretch)
	IMAGICK_REGISTER_CONST_LONG("STRETCH_NORMAL", NormalStretch);
#endif
#if defined(UltraCondensedStretch)
	IMAGICK_REGISTER_CONST_LONG("STRETCH_ULTRACONDENSED", UltraCondensedStretch);
#endif
#if defined(CondensedStretch)
	IMAGICK_REGISTER_CONST_LONG("STRETCH_CONDENSED", CondensedStretch);
#endif
#if defined(SemiCondensedStretch)
	IMAGICK_REGISTER_CONST_LONG("STRETCH_SEMICONDENSED", SemiCondensedStretch);
#endif
#if defined(SemiExpandedStretch)
	IMAGICK_REGISTER_CONST_LONG("STRETCH_SEMIEXPANDED", SemiExpandedStretch);
#endif
#if defined(ExpandedStretch)
	IMAGICK_REGISTER_CONST_LONG("STRETCH_EXPANDED", ExpandedStretch);
#endif
#if defined(ExtraExpandedStretch)
	IMAGICK_REGISTER_CONST_LONG("STRETCH_EXTRAEXPANDED", ExtraExpandedStretch);
#endif
#if defined(UltraExpandedStretch)
	IMAGICK_REGISTER_CONST_LONG("STRETCH_ULTRAEXPANDED", UltraExpandedStretch);
#endif
#if defined(AnyStretch)
	IMAGICK_REGISTER_CONST_LONG("STRETCH_ANY", AnyStretch);
#endif
#if defined(UndefinedAlign)
	IMAGICK_REGISTER_CONST_LONG("ALIGN_UNDEFINED", UndefinedAlign);
#endif
#if defined(LeftAlign)
	IMAGICK_REGISTER_CONST_LONG("ALIGN_LEFT", LeftAlign);
#endif
#if defined(CenterAlign)
	IMAGICK_REGISTER_CONST_LONG("ALIGN_CENTER", CenterAlign);
#endif
#if defined(RightAlign)
	IMAGICK_REGISTER_CONST_LONG("ALIGN_RIGHT",	RightAlign);
#endif
#if defined(NoDecoration)
	IMAGICK_REGISTER_CONST_LONG("DECORATION_NO", NoDecoration);
#endif
#if defined(UnderlineDecoration)
	IMAGICK_REGISTER_CONST_LONG("DECORATION_UNDERLINE", UnderlineDecoration);
#endif
#if defined(OverlineDecoration)
	IMAGICK_REGISTER_CONST_LONG("DECORATION_OVERLINE", OverlineDecoration);
#endif
#if defined(LineThroughDecoration)
	IMAGICK_REGISTER_CONST_LONG("DECORATION_LINETROUGH", LineThroughDecoration);
#endif
#if defined(UniformNoise)
	IMAGICK_REGISTER_CONST_LONG("NOISE_UNIFORM", UniformNoise);
#endif
#if defined(GaussianNoise)
	IMAGICK_REGISTER_CONST_LONG("NOISE_GAUSSIAN", GaussianNoise);
#endif
#if defined(MultiplicativeGaussianNoise)
	IMAGICK_REGISTER_CONST_LONG("NOISE_MULTIPLICATIVEGAUSSIAN", MultiplicativeGaussianNoise);
#endif
#if defined(ImpulseNoise)
	IMAGICK_REGISTER_CONST_LONG("NOISE_IMPULSE", ImpulseNoise);
#endif
#if defined(LaplacianNoise)
	IMAGICK_REGISTER_CONST_LONG("NOISE_LAPLACIAN", LaplacianNoise );
#endif
#if defined(PoissonNoise)
	IMAGICK_REGISTER_CONST_LONG("NOISE_POISSON", PoissonNoise);
#endif
#if defined(RandomNoise)
	IMAGICK_REGISTER_CONST_LONG("NOISE_RANDOM", RandomNoise);
#endif
#if defined(UndefinedChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_UNDEFINED", UndefinedChannel);
#endif
#if defined(RedChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_RED", RedChannel);
#endif
#if defined(GrayChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_GRAY", GrayChannel);
#endif
#if defined(CyanChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_CYAN", CyanChannel);
#endif
#if defined(GreenChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_GREEN", GreenChannel);
#endif
#if defined(MagentaChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_MAGENTA", MagentaChannel);
#endif
#if defined(BlueChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_BLUE", BlueChannel);
#endif
#if defined(YellowChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_YELLOW", YellowChannel);
#endif
#if defined(AlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_ALPHA", AlphaChannel);
#endif
#if defined(OpacityChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_OPACITY", OpacityChannel);
#endif
#if defined(MatteChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_MATTE", MatteChannel);
#endif
#if defined(BlackChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_BLACK", BlackChannel);
#endif
#if defined(IndexChannel)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_INDEX", IndexChannel);
#endif
#if defined(AllChannels)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_ALL", AllChannels);
#endif
#if defined(DefaultChannels)
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_DEFAULT", DefaultChannels);
#endif
#if defined(UndefinedMetric)
	IMAGICK_REGISTER_CONST_LONG("METRIC_UNDEFINED", UndefinedMetric);
#endif
#if defined(MeanAbsoluteErrorMetric)
	IMAGICK_REGISTER_CONST_LONG("METRIC_MEANABSOLUTEERROR", MeanAbsoluteErrorMetric);
#endif
#if defined(MeanSquaredErrorMetric)
	IMAGICK_REGISTER_CONST_LONG("METRIC_MEANSQUAREERROR", MeanSquaredErrorMetric);
#endif
#if defined(PeakAbsoluteErrorMetric)
	IMAGICK_REGISTER_CONST_LONG("METRIC_PEAKABSOLUTEERROR", PeakAbsoluteErrorMetric);
#endif
#if defined(PeakSignalToNoiseRatioMetric)
	IMAGICK_REGISTER_CONST_LONG("METRIC_PEAKSIGNALTONOISERATIO", PeakSignalToNoiseRatioMetric);
#endif
#if defined(RootMeanSquaredErrorMetric)
	IMAGICK_REGISTER_CONST_LONG("METRIC_ROOTMEANSQUAREDERROR", RootMeanSquaredErrorMetric);
#endif
#if defined(CharPixel)
	IMAGICK_REGISTER_CONST_LONG("PIXEL_CHAR", CharPixel);
#endif
#if defined(DoublePixel)
	IMAGICK_REGISTER_CONST_LONG("PIXEL_DOUBLE", DoublePixel);
#endif
#if defined(FloatPixel)
	IMAGICK_REGISTER_CONST_LONG("PIXEL_FLOAT", FloatPixel);
#endif
#if defined(IntegerPixel)
	IMAGICK_REGISTER_CONST_LONG("PIXEL_INTEGER", IntegerPixel);
#endif
#if defined(LongPixel)
	IMAGICK_REGISTER_CONST_LONG("PIXEL_LONG", LongPixel);
#endif
#if defined(QuantumPixel)
	IMAGICK_REGISTER_CONST_LONG("PIXEL_QUANTUM", QuantumPixel);
#endif
#if defined(ShortPixel)
	IMAGICK_REGISTER_CONST_LONG("PIXEL_SHORT", ShortPixel);
#endif
#if defined(UndefinedEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_UNDEFINED", UndefinedEvaluateOperator);
#endif
#if defined(AddEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_ADD", AddEvaluateOperator);
#endif
#if defined(AndEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_AND", AndEvaluateOperator);
#endif
#if defined(DivideEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_DIVIDE", DivideEvaluateOperator);
#endif
#if defined(LeftShiftEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_LEFTSHIFT", LeftShiftEvaluateOperator);
#endif
#if defined(MaxEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_MAX", MaxEvaluateOperator);
#endif
#if defined(MinEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_MIN", MinEvaluateOperator);
#endif
#if defined(MultiplyEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_MULTIPLY", MultiplyEvaluateOperator);
#endif
#if defined(OrEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_OR", OrEvaluateOperator);
#endif
#if defined(RightShiftEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_RIGHTSHIFT", RightShiftEvaluateOperator);
#endif
#if defined(SetEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_SET", SetEvaluateOperator);
#endif
#if defined(SubtractEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_SUBTRACT", SubtractEvaluateOperator);
#endif
#if defined(XorEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_XOR", XorEvaluateOperator);
#endif
#if defined(PowEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_POW", PowEvaluateOperator);
#endif
#if defined(LogEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_LOG", LogEvaluateOperator);
#endif
#if defined(ThresholdEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_THRESHOLD", ThresholdEvaluateOperator);
#endif
#if defined(ThresholdBlackEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_THRESHOLDBLACK", ThresholdBlackEvaluateOperator);
#endif
#if defined(ThresholdWhiteEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_THRESHOLDWHITE", ThresholdWhiteEvaluateOperator);
#endif
#if defined(GaussianNoiseEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_GAUSSIANNOISE", GaussianNoiseEvaluateOperator);
#endif
#if defined(ImpulseNoiseEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_IMPULSENOISE", ImpulseNoiseEvaluateOperator);
#endif
#if defined(LaplacianNoiseEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_LAPLACIANNOISE", LaplacianNoiseEvaluateOperator);
#endif
#if defined(MultiplicativeNoiseEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_MULTIPLICATIVENOISE", MultiplicativeNoiseEvaluateOperator);
#endif
#if defined(PoissonNoiseEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_POISSONNOISE", PoissonNoiseEvaluateOperator);
#endif
#if defined(UniformNoiseEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_UNIFORMNOISE", UniformNoiseEvaluateOperator);
#endif
#if defined(CosineEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_COSINE", CosineEvaluateOperator);
#endif
#if defined(SineEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_SINE", SineEvaluateOperator);
#endif
#if defined(AddModulusEvaluateOperator)
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_ADDMODULUS", AddModulusEvaluateOperator);
#endif
#if defined(UndefinedColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_UNDEFINED", UndefinedColorspace);
#endif
#if defined(RGBColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_RGB", RGBColorspace);
#endif
#if defined(GRAYColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_GRAY", GRAYColorspace);
#endif
#if defined(TransparentColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_TRANSPARENT", TransparentColorspace);
#endif
#if defined(OHTAColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_OHTA", OHTAColorspace);
#endif
#if defined(LABColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_LAB", LABColorspace);
#endif
#if defined(XYZColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_XYZ", XYZColorspace);
#endif
#if defined(YCbCrColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_YCBCR", YCbCrColorspace);
#endif
#if defined(YCCColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_YCC", YCCColorspace);
#endif
#if defined(YIQColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_YIQ", YIQColorspace);
#endif
#if defined(YPbPrColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_YPBPR", YPbPrColorspace);
#endif
#if defined(YUVColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_YUV", YUVColorspace);
#endif
#if defined(CMYKColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_CMYK", CMYKColorspace);
#endif
#if defined(sRGBColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_SRGB", sRGBColorspace);
#endif
#if defined(HSBColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_HSB", HSBColorspace);
#endif
#if defined(HSLColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_HSL", HSLColorspace);
#endif
#if defined(HWBColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_HWB", HWBColorspace);
#endif
#if defined(Rec601LumaColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_REC601LUMA", Rec601LumaColorspace);
#endif
#if defined(Rec709LumaColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_REC709LUMA", Rec709LumaColorspace);
#endif
#if defined(LogColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_LOG", LogColorspace);
#endif
#if defined(CMYColorspace)
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_CMY", CMYColorspace);
#endif
#if defined(UndefinedVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_UNDEFINED", UndefinedVirtualPixelMethod);
#endif
#if defined(BackgroundVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_BACKGROUND", BackgroundVirtualPixelMethod);
#endif
#if defined(ConstantVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_CONSTANT", ConstantVirtualPixelMethod);
#endif
#if defined(EdgeVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_EDGE", EdgeVirtualPixelMethod);
#endif
#if defined(MirrorVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_MIRROR", MirrorVirtualPixelMethod);
#endif
#if defined(TileVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_TILE", TileVirtualPixelMethod);
#endif
#if defined(TransparentVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_TRANSPARENT", TransparentVirtualPixelMethod);
#endif
#if defined(MaskVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_MASK", MaskVirtualPixelMethod);
#endif
#if defined(BlackVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_BLACK", BlackVirtualPixelMethod);
#endif
#if defined(GrayVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_GRAY", GrayVirtualPixelMethod);
#endif
#if defined(WhiteVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_WHITE", WhiteVirtualPixelMethod);
#endif
#if defined(HorizontalTileVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_HORIZONTALTILE", HorizontalTileVirtualPixelMethod);
#endif
#if defined(VerticalTileVirtualPixelMethod)
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_VERTICALTILE", VerticalTileVirtualPixelMethod);
#endif
#if defined(UndefinedPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_UNDEFINED", UndefinedPreview);
#endif
#if defined(RotatePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_ROTATE", RotatePreview);
#endif
#if defined(ShearPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SHEAR", ShearPreview);
#endif
#if defined(RollPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_ROLL", RollPreview);
#endif
#if defined(HuePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_HUE", HuePreview);
#endif
#if defined(SaturationPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SATURATION", SaturationPreview);
#endif
#if defined(BrightnessPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_BRIGHTNESS", BrightnessPreview);
#endif
#if defined(GammaPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_GAMMA", GammaPreview);
#endif
#if defined(SpiffPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SPIFF", SpiffPreview);
#endif
#if defined(DullPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_DULL", DullPreview);
#endif
#if defined(GrayscalePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_GRAYSCALE", GrayscalePreview);
#endif
#if defined(QuantizePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_QUANTIZE", QuantizePreview);
#endif
#if defined(DespecklePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_DESPECKLE", DespecklePreview);
#endif
#if defined(ReduceNoisePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_REDUCENOISE", ReduceNoisePreview);
#endif
#if defined(AddNoisePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_ADDNOISE", AddNoisePreview);
#endif
#if defined(SharpenPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SHARPEN", SharpenPreview);
#endif
#if defined(BlurPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_BLUR", BlurPreview);
#endif
#if defined(ThresholdPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_THRESHOLD", ThresholdPreview);
#endif
#if defined(EdgeDetectPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_EDGEDETECT", EdgeDetectPreview);
#endif
#if defined(SpreadPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SPREAD", SpreadPreview);
#endif
#if defined(SolarizePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SOLARIZE", SolarizePreview);
#endif
#if defined(ShadePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SHADE", ShadePreview);
#endif
#if defined(RaisePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_RAISE", RaisePreview);
#endif
#if defined(SegmentPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SEGMENT", SegmentPreview);
#endif
#if defined(SwirlPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SWIRL", SwirlPreview);
#endif
#if defined(ImplodePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_IMPLODE", ImplodePreview);
#endif
#if defined(WavePreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_WAVE", WavePreview);
#endif
#if defined(OilPaintPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_OILPAINT", OilPaintPreview);
#endif
#if defined(CharcoalDrawingPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_CHARCOALDRAWING", CharcoalDrawingPreview);
#endif
#if defined(JPEGPreview)
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_JPEG", JPEGPreview);
#endif
#if defined(UndefinedIntent)
	IMAGICK_REGISTER_CONST_LONG("RENDERINGINTENT_UNDEFINED", UndefinedIntent);
#endif
#if defined(SaturationIntent)
	IMAGICK_REGISTER_CONST_LONG("RENDERINGINTENT_SATURATION", SaturationIntent);
#endif
#if defined(PerceptualIntent)
	IMAGICK_REGISTER_CONST_LONG("RENDERINGINTENT_PERCEPTUAL", PerceptualIntent);
#endif
#if defined(AbsoluteIntent)
	IMAGICK_REGISTER_CONST_LONG("RENDERINGINTENT_ABSOLUTE", AbsoluteIntent);
#endif
#if defined(RelativeIntent)
	IMAGICK_REGISTER_CONST_LONG("RENDERINGINTENT_RELATIVE", RelativeIntent);
#endif
#if defined(UndefinedInterlace)
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_UNDEFINED", UndefinedInterlace);
#endif
#if defined(NoInterlace)
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_NO", NoInterlace);
#endif
#if defined(LineInterlace)
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_LINE", LineInterlace);
#endif
#if defined(PlaneInterlace)
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_PLANE", PlaneInterlace);
#endif
#if defined(PartitionInterlace)
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_PARTITION", PartitionInterlace);
#endif
#if defined(GIFInterlace)
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_GIF", GIFInterlace);
#endif
#if defined(JPEGInterlace)
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_JPEG", JPEGInterlace);
#endif
#if defined(PNGInterlace)
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_PNG", PNGInterlace);
#endif
#if defined(UndefinedRule)
	IMAGICK_REGISTER_CONST_LONG("FILLRULE_UNDEFINED", UndefinedRule);
#endif
#if defined(EvenOddRule)
	IMAGICK_REGISTER_CONST_LONG("FILLRULE_EVENODD", EvenOddRule);
#endif
#if defined(NonZeroRule)
	IMAGICK_REGISTER_CONST_LONG("FILLRULE_NONZERO", NonZeroRule);
#endif
#if defined(UndefinedPathUnits)
	IMAGICK_REGISTER_CONST_LONG("PATHUNITS_UNDEFINED", UndefinedPathUnits);
#endif
#if defined(UserSpace)
	IMAGICK_REGISTER_CONST_LONG("PATHUNITS_USERSPACE", UserSpace);
#endif
#if defined(UserSpaceOnUse)
	IMAGICK_REGISTER_CONST_LONG("PATHUNITS_USERSPACEONUSE", UserSpaceOnUse);
#endif
#if defined(ObjectBoundingBox)
	IMAGICK_REGISTER_CONST_LONG("PATHUNITS_OBJECTBOUNDINGBOX", ObjectBoundingBox);
#endif
#if defined(UndefinedCap)
	IMAGICK_REGISTER_CONST_LONG("LINECAP_UNDEFINED", UndefinedCap);
#endif
#if defined(ButtCap)
	IMAGICK_REGISTER_CONST_LONG("LINECAP_BUTT", ButtCap);
#endif
#if defined(RoundCap)
	IMAGICK_REGISTER_CONST_LONG("LINECAP_ROUND", RoundCap);
#endif
#if defined(SquareCap)
	IMAGICK_REGISTER_CONST_LONG("LINECAP_SQUARE", SquareCap);
#endif
#if defined(UndefinedJoin)
	IMAGICK_REGISTER_CONST_LONG("LINEJOIN_UNDEFINED", UndefinedJoin);
#endif
#if defined(MiterJoin)
	IMAGICK_REGISTER_CONST_LONG("LINEJOIN_MITER", MiterJoin);
#endif
#if defined(RoundJoin)
	IMAGICK_REGISTER_CONST_LONG("LINEJOIN_ROUND", RoundJoin);
#endif
#if defined(BevelJoin)
	IMAGICK_REGISTER_CONST_LONG("LINEJOIN_BEVEL", BevelJoin);
#endif
#if defined(UndefinedResource)
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_UNDEFINED", UndefinedResource);
#endif
#if defined(AreaResource)
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_AREA", AreaResource);
#endif
#if defined(DiskResource)
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_DISK", DiskResource);
#endif
#if defined(FileResource)
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_FILE", FileResource);
#endif
#if defined(MapResource)
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_MAP", MapResource);
#endif
#if defined(MemoryResource)
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_MEMORY", MemoryResource);
#endif
#if defined(UnrecognizedDispose)
	IMAGICK_REGISTER_CONST_LONG("DISPOSE_UNRECOGNIZED", UnrecognizedDispose);
#endif
#if defined(UndefinedDispose)
	IMAGICK_REGISTER_CONST_LONG("DISPOSE_UNDEFINED", UndefinedDispose);
#endif
#if defined(NoneDispose)
	IMAGICK_REGISTER_CONST_LONG("DISPOSE_NONE", NoneDispose);
#endif
#if defined(BackgroundDispose)
	IMAGICK_REGISTER_CONST_LONG("DISPOSE_BACKGROUND", BackgroundDispose);
#endif
#if defined(PreviousDispose)
	IMAGICK_REGISTER_CONST_LONG("DISPOSE_PREVIOUS", PreviousDispose);
#endif
#if defined(UndefinedInterpolatePixel)
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_UNDEFINED", UndefinedInterpolatePixel);
#endif
#if defined(AverageInterpolatePixel)
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_AVERAGE", AverageInterpolatePixel);
#endif
#if defined(BicubicInterpolatePixel)
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_BICUBIC", BicubicInterpolatePixel);
#endif
#if defined(BilinearInterpolatePixel)
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_BILINEAR", BilinearInterpolatePixel);
#endif
#if defined(FilterInterpolatePixel)
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_FILTER", FilterInterpolatePixel);
#endif
#if defined(IntegerInterpolatePixel)
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_INTEGER", IntegerInterpolatePixel);
#endif
#if defined(MeshInterpolatePixel)
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_MESH", MeshInterpolatePixel);
#endif
#if defined(NearestNeighborInterpolatePixel)
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_NEARESTNEIGHBOR", NearestNeighborInterpolatePixel);
#endif
#if defined(SplineInterpolatePixel)
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_SPLINE", SplineInterpolatePixel);
#endif
#if defined(UndefinedLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_UNDEFINED", UndefinedLayer);
#endif
#if defined(CoalesceLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_COALESCE", CoalesceLayer);
#endif
#if defined(CompareAnyLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_COMPAREANY", CompareAnyLayer);
#endif
#if defined(CompareClearLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_COMPARECLEAR", CompareClearLayer);
#endif
#if defined(CompareOverlayLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_COMPAREOVERLAY", CompareOverlayLayer);
#endif
#if defined(DisposeLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_DISPOSE", DisposeLayer);
#endif
#if defined(OptimizeLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_OPTIMIZE", OptimizeLayer);
#endif
#if defined(OptimizePlusLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_OPTIMIZEPLUS", OptimizePlusLayer);
#endif
#if defined(OptimizeImageLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_OPTIMIZEIMAGE", OptimizeImageLayer);
#endif
#if defined(OptimizeTransLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_OPTIMIZETRANS", OptimizeTransLayer);
#endif
#if defined(RemoveDupsLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_REMOVEDUPS", RemoveDupsLayer);
#endif
#if defined(RemoveZeroLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_REMOVEZERO", RemoveZeroLayer);
#endif
#if defined(CompositeLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_COMPOSITE", CompositeLayer);
#endif
#if defined(UndefinedOrientation)
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_UNDEFINED", UndefinedOrientation);
#endif
#if defined(TopLeftOrientation)
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_TOPLEFT", TopLeftOrientation);
#endif
#if defined(TopRightOrientation)
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_TOPRIGHT", TopRightOrientation);
#endif
#if defined(BottomRightOrientation)
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_BOTTOMRIGHT", BottomRightOrientation);
#endif
#if defined(BottomLeftOrientation)
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_BOTTOMLEFT", BottomLeftOrientation);
#endif
#if defined(LeftTopOrientation)
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_LEFTTOP", LeftTopOrientation);
#endif
#if defined(RightTopOrientation)
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_RIGHTTOP", RightTopOrientation);
#endif
#if defined(RightBottomOrientation)
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_RIGHTBOTTOM", RightBottomOrientation);
#endif
#if defined(LeftBottomOrientation)
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_LEFTBOTTOM", LeftBottomOrientation);
#endif
#if defined(UndefinedDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_UNDEFINED", UndefinedDistortion);
#endif
#if defined(AffineDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_AFFINE", AffineDistortion);
#endif
#if defined(AffineProjectionDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_AFFINEPROJECTION", AffineProjectionDistortion);
#endif
#if defined(ArcDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_ARC", ArcDistortion);
#endif
#if defined(BilinearDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_BILINEAR", BilinearDistortion);
#endif
#if defined(PerspectiveDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_PERSPECTIVE", PerspectiveDistortion);
#endif
#if defined(PerspectiveProjectionDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_PERSPECTIVEPROJECTION", PerspectiveProjectionDistortion);
#endif
#if defined(ScaleRotateTranslateDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_SCALEROTATETRANSLATE", ScaleRotateTranslateDistortion);
#endif
#if defined(PolynomialDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_POLYNOMIAL", PolynomialDistortion);
#endif
#if defined(PolarDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_POLAR", PolarDistortion);
#endif
#if defined(DePolarDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_DEPOLAR", DePolarDistortion);
#endif
#if defined(BarrelDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_BARREL", BarrelDistortion);
#endif
#if defined(BarrelInverseDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_BARRELINVERSE", BarrelInverseDistortion);
#endif
#if defined(ShepardsDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_SHEPARDS", ShepardsDistortion);
#endif
#if defined(SentinelDistortion)
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_SENTINEL", SentinelDistortion);
#endif
#if defined(MergeLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_MERGE", MergeLayer);
#endif
#if defined(FlattenLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_FLATTEN", FlattenLayer);
#endif
#if defined(MosaicLayer)
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_MOSAIC", MosaicLayer);
#endif
#if defined(ActivateAlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_ACTIVATE", ActivateAlphaChannel);
#endif
#if defined(DeactivateAlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_DEACTIVATE", DeactivateAlphaChannel);
#endif
#if defined(ResetAlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_RESET", ResetAlphaChannel);
#endif
#if defined(SetAlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_SET", SetAlphaChannel);
#endif
#if defined(UndefinedAlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_UNDEFINED", UndefinedAlphaChannel);
#endif
#if defined(CopyAlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_COPY", CopyAlphaChannel);
#endif
#if defined(DeactivateAlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_DEACTIVATE", DeactivateAlphaChannel);
#endif
#if defined(ExtractAlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_EXTRACT", ExtractAlphaChannel);
#endif
#if defined(OpaqueAlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_OPAQUE", OpaqueAlphaChannel);
#endif
#if defined(ShapeAlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_SHAPE", ShapeAlphaChannel);
#endif
#if defined(TransparentAlphaChannel)
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_TRANSPARENT", TransparentAlphaChannel);
#endif
#if defined(UndefinedColorInterpolate)
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_UNDEFINED", UndefinedColorInterpolate);
#endif
#if defined(BarycentricColorInterpolate)
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_BARYCENTRIC", BarycentricColorInterpolate);
#endif
#if defined(BilinearColorInterpolate)
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_BILINEAR", BilinearColorInterpolate);
#endif
#if defined(PolynomialColorInterpolate)
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_POLYNOMIAL", PolynomialColorInterpolate);
#endif
#if defined(ShepardsColorInterpolate)
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_SPEPARDS", ShepardsColorInterpolate);
#endif
#if defined(VoronoiColorInterpolate)
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_VORONOI", VoronoiColorInterpolate);
#endif
#if defined(UndefinedDitherMethod)
	IMAGICK_REGISTER_CONST_LONG("DITHERMETHOD_UNDEFINED", UndefinedDitherMethod);
#endif
#if defined(NoDitherMethod)
	IMAGICK_REGISTER_CONST_LONG("DITHERMETHOD_NO", NoDitherMethod);
#endif
#if defined(RiemersmaDitherMethod)
	IMAGICK_REGISTER_CONST_LONG("DITHERMETHOD_RIEMERSMA", RiemersmaDitherMethod);
#endif
#if defined(FloydSteinbergDitherMethod)
	IMAGICK_REGISTER_CONST_LONG("DITHERMETHOD_FLOYDSTEINBERG", FloydSteinbergDitherMethod);
#endif
#if defined(UndefinedFunction)
	IMAGICK_REGISTER_CONST_LONG("FUNCTION_UNDEFINED", UndefinedFunction);
#endif
#if defined(PolynomialFunction)
	IMAGICK_REGISTER_CONST_LONG("FUNCTION_POLYNOMIAL", PolynomialFunction);
#endif
#if defined(SinusoidFunction)
	IMAGICK_REGISTER_CONST_LONG("FUNCTION_SINUSOID", SinusoidFunction);
#endif
}
