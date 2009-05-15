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
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DEFAULT", OverCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_UNDEFINED", UndefinedCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_NO", NoCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_ADD", AddCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_ATOP", AtopCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_BLEND", BlendCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_BUMPMAP", BumpmapCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_CLEAR", ClearCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COLORBURN", ColorBurnCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COLORDODGE", ColorDodgeCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COLORIZE", ColorizeCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYBLACK", CopyBlackCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYBLUE", CopyBlueCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPY", CopyCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYCYAN", CopyCyanCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYGREEN", CopyGreenCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYMAGENTA", CopyMagentaCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYOPACITY", CopyOpacityCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYRED", CopyRedCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_COPYYELLOW", CopyYellowCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DARKEN", DarkenCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DSTATOP", DstAtopCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DST", DstCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DSTIN", DstInCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DSTOUT", DstOutCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DSTOVER", DstOverCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DIFFERENCE", DifferenceCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DISPLACE", DisplaceCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_DISSOLVE", DissolveCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_EXCLUSION", ExclusionCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_HARDLIGHT", HardLightCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_HUE", HueCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_IN", InCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_LIGHTEN", LightenCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_LUMINIZE", LuminizeCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_MINUS", MinusCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_MODULATE", ModulateCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_MULTIPLY", MultiplyCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_OUT", OutCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_OVER", OverCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_OVERLAY", OverlayCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_PLUS", PlusCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_REPLACE", ReplaceCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SATURATE", SaturateCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SCREEN", ScreenCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SOFTLIGHT", SoftLightCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SRCATOP", SrcAtopCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SRC", SrcCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SRCIN", SrcInCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SRCOUT", SrcOutCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SRCOVER", SrcOverCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_SUBTRACT", SubtractCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_THRESHOLD", ThresholdCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("COMPOSITE_XOR", XorCompositeOp);
	IMAGICK_REGISTER_CONST_LONG("MONTAGEMODE_FRAME", FrameMode);
	IMAGICK_REGISTER_CONST_LONG("MONTAGEMODE_UNFRAME", UnframeMode);
	IMAGICK_REGISTER_CONST_LONG("MONTAGEMODE_CONCATENATE", ConcatenateMode);
	IMAGICK_REGISTER_CONST_LONG("STYLE_NORMAL", NormalStyle);
	IMAGICK_REGISTER_CONST_LONG("STYLE_ITALIC", ItalicStyle);
	IMAGICK_REGISTER_CONST_LONG("STYLE_OBLIQUE", ObliqueStyle);
	IMAGICK_REGISTER_CONST_LONG("STYLE_ANY", AnyStyle);
	IMAGICK_REGISTER_CONST_LONG("FILTER_UNDEFINED", UndefinedFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_POINT", PointFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_BOX", BoxFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_TRIANGLE", TriangleFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_HERMITE", HermiteFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_HANNING", HanningFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_HAMMING", HammingFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_BLACKMAN", BlackmanFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_GAUSSIAN", GaussianFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_QUADRATIC", QuadraticFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_CUBIC", CubicFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_CATROM", CatromFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_MITCHELL", MitchellFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_LANCZOS", LanczosFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_BESSEL", BesselFilter);
	IMAGICK_REGISTER_CONST_LONG("FILTER_SINC", SincFilter);
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_UNDEFINED", UndefinedType);
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_BILEVEL", BilevelType);
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_GRAYSCALE", GrayscaleType);
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_GRAYSCALEMATTE", GrayscaleMatteType);
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_PALETTE",  PaletteType);
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_PALETTEMATTE", PaletteMatteType);
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_TRUECOLOR", TrueColorType);
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_TRUECOLORMATTE", TrueColorMatteType);
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_COLORSEPARATION", ColorSeparationType);
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_COLORSEPARATIONMATTE", ColorSeparationMatteType);
	IMAGICK_REGISTER_CONST_LONG("IMGTYPE_OPTIMIZE", OptimizeType);
	IMAGICK_REGISTER_CONST_LONG("RESOLUTION_UNDEFINED", UndefinedResolution);
	IMAGICK_REGISTER_CONST_LONG("RESOLUTION_PIXELSPERINCH", PixelsPerInchResolution);
	IMAGICK_REGISTER_CONST_LONG("RESOLUTION_PIXELSPERCENTIMETER", PixelsPerCentimeterResolution);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_UNDEFINED", UndefinedCompression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_NO", NoCompression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_BZIP", BZipCompression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_FAX", FaxCompression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_GROUP4", Group4Compression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_JPEG", JPEGCompression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_JPEG2000", JPEG2000Compression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_LOSSLESSJPEG", LosslessJPEGCompression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_LZW", LZWCompression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_RLE", RLECompression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_ZIP", ZipCompression);
#if MagickLibVersion > 0x639
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_DXT1", DXT1Compression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_DXT3", DXT3Compression);
	IMAGICK_REGISTER_CONST_LONG("COMPRESSION_DXT5", DXT5Compression);
#endif
	IMAGICK_REGISTER_CONST_LONG("PAINT_POINT", PointMethod);
	IMAGICK_REGISTER_CONST_LONG("PAINT_REPLACE", ReplaceMethod);
	IMAGICK_REGISTER_CONST_LONG("PAINT_FLOODFILL", FloodfillMethod);
	IMAGICK_REGISTER_CONST_LONG("PAINT_FILLTOBORDER", FillToBorderMethod);
	IMAGICK_REGISTER_CONST_LONG("PAINT_RESET", ResetMethod);
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_NORTHWEST", NorthWestGravity);
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_NORTH", NorthGravity);
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_NORTHEAST", NorthEastGravity);
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_WEST", WestGravity);
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_CENTER", CenterGravity);
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_EAST", EastGravity);
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_SOUTHWEST", SouthWestGravity);
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_SOUTH", SouthGravity);
	IMAGICK_REGISTER_CONST_LONG("GRAVITY_SOUTHEAST", SouthEastGravity);
	IMAGICK_REGISTER_CONST_LONG("STRETCH_NORMAL", NormalStretch);
	IMAGICK_REGISTER_CONST_LONG("STRETCH_ULTRACONDENSED", UltraCondensedStretch);
	IMAGICK_REGISTER_CONST_LONG("STRETCH_CONDENSED", CondensedStretch);
	IMAGICK_REGISTER_CONST_LONG("STRETCH_SEMICONDENSED", SemiCondensedStretch);
	IMAGICK_REGISTER_CONST_LONG("STRETCH_SEMIEXPANDED", SemiExpandedStretch);
	IMAGICK_REGISTER_CONST_LONG("STRETCH_EXPANDED", ExpandedStretch);
	IMAGICK_REGISTER_CONST_LONG("STRETCH_EXTRAEXPANDED", ExtraExpandedStretch);
	IMAGICK_REGISTER_CONST_LONG("STRETCH_ULTRAEXPANDED", UltraExpandedStretch);
	IMAGICK_REGISTER_CONST_LONG("STRETCH_ANY", AnyStretch);
	IMAGICK_REGISTER_CONST_LONG("ALIGN_UNDEFINED", UndefinedAlign);
	IMAGICK_REGISTER_CONST_LONG("ALIGN_LEFT", LeftAlign);
	IMAGICK_REGISTER_CONST_LONG("ALIGN_CENTER", CenterAlign);
	IMAGICK_REGISTER_CONST_LONG("ALIGN_RIGHT",	RightAlign);
	IMAGICK_REGISTER_CONST_LONG("DECORATION_NO", NoDecoration);
	IMAGICK_REGISTER_CONST_LONG("DECORATION_UNDERLINE", UnderlineDecoration);
	IMAGICK_REGISTER_CONST_LONG("DECORATION_OVERLINE", OverlineDecoration);
	IMAGICK_REGISTER_CONST_LONG("DECORATION_LINETROUGH", LineThroughDecoration);
	IMAGICK_REGISTER_CONST_LONG("NOISE_UNIFORM", UniformNoise);
	IMAGICK_REGISTER_CONST_LONG("NOISE_GAUSSIAN", GaussianNoise);
	IMAGICK_REGISTER_CONST_LONG("NOISE_MULTIPLICATIVEGAUSSIAN", MultiplicativeGaussianNoise);
	IMAGICK_REGISTER_CONST_LONG("NOISE_IMPULSE", ImpulseNoise);
	IMAGICK_REGISTER_CONST_LONG("NOISE_LAPLACIAN", LaplacianNoise);
	IMAGICK_REGISTER_CONST_LONG("NOISE_POISSON", PoissonNoise);
#if MagickLibVersion > 0x635
	IMAGICK_REGISTER_CONST_LONG("NOISE_RANDOM", RandomNoise);
#endif
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_UNDEFINED", UndefinedChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_RED", RedChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_GRAY", GrayChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_CYAN", CyanChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_GREEN", GreenChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_MAGENTA", MagentaChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_BLUE", BlueChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_YELLOW", YellowChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_ALPHA", AlphaChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_OPACITY", OpacityChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_MATTE", MatteChannel); /* deprecated, needs to throw E_STRICT if used */
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_BLACK", BlackChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_INDEX", IndexChannel);
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_ALL", AllChannels);
#ifdef DefaultChannels
	IMAGICK_REGISTER_CONST_LONG("CHANNEL_DEFAULT", DefaultChannels);
#endif	
	IMAGICK_REGISTER_CONST_LONG("METRIC_UNDEFINED", UndefinedMetric);
	IMAGICK_REGISTER_CONST_LONG("METRIC_MEANABSOLUTEERROR", MeanAbsoluteErrorMetric);
	IMAGICK_REGISTER_CONST_LONG("METRIC_MEANSQUAREERROR", MeanSquaredErrorMetric);
	IMAGICK_REGISTER_CONST_LONG("METRIC_PEAKABSOLUTEERROR", PeakAbsoluteErrorMetric);
	IMAGICK_REGISTER_CONST_LONG("METRIC_PEAKSIGNALTONOISERATIO", PeakSignalToNoiseRatioMetric);
	IMAGICK_REGISTER_CONST_LONG("METRIC_ROOTMEANSQUAREDERROR", RootMeanSquaredErrorMetric);
	IMAGICK_REGISTER_CONST_LONG("PIXEL_CHAR", CharPixel);
	IMAGICK_REGISTER_CONST_LONG("PIXEL_DOUBLE", DoublePixel);
	IMAGICK_REGISTER_CONST_LONG("PIXEL_FLOAT", FloatPixel);
	IMAGICK_REGISTER_CONST_LONG("PIXEL_INTEGER", IntegerPixel);
	IMAGICK_REGISTER_CONST_LONG("PIXEL_LONG", LongPixel);
	IMAGICK_REGISTER_CONST_LONG("PIXEL_QUANTUM", QuantumPixel);
	IMAGICK_REGISTER_CONST_LONG("PIXEL_SHORT", ShortPixel);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_UNDEFINED", UndefinedEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_ADD", AddEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_AND", AndEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_DIVIDE", DivideEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_LEFTSHIFT", LeftShiftEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_MAX", MaxEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_MIN", MinEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_MULTIPLY", MultiplyEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_OR", OrEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_RIGHTSHIFT", RightShiftEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_SET", SetEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_SUBTRACT", SubtractEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_XOR", XorEvaluateOperator);
#if MagickLibVersion > 0x643
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_POW", PowEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_LOG", LogEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_THRESHOLD", ThresholdEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_THRESHOLDBLACK", ThresholdBlackEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_THRESHOLDWHITE", ThresholdWhiteEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_GAUSSIANNOISE", GaussianNoiseEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_IMPULSENOISE", ImpulseNoiseEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_LAPLACIANNOISE", LaplacianNoiseEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_MULTIPLICATIVENOISE", MultiplicativeNoiseEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_POISSONNOISE", PoissonNoiseEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_UNIFORMNOISE", UniformNoiseEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_COSINE", CosineEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_SINE", SineEvaluateOperator);
	IMAGICK_REGISTER_CONST_LONG("EVALUATE_ADDMODULUS", AddModulusEvaluateOperator);
#endif	
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_UNDEFINED", UndefinedColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_RGB", RGBColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_GRAY", GRAYColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_TRANSPARENT", TransparentColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_OHTA", OHTAColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_LAB", LABColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_XYZ", XYZColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_YCBCR", YCbCrColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_YCC", YCCColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_YIQ", YIQColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_YPBPR", YPbPrColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_YUV", YUVColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_CMYK", CMYKColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_SRGB", sRGBColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_HSB", HSBColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_HSL", HSLColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_HWB", HWBColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_REC601LUMA", Rec601LumaColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_REC709LUMA", Rec709LumaColorspace);
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_LOG", LogColorspace);
#if MagickLibVersion >= 0x642
	IMAGICK_REGISTER_CONST_LONG("COLORSPACE_CMY", CMYColorspace);
#endif
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_UNDEFINED", UndefinedVirtualPixelMethod);
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_BACKGROUND", BackgroundVirtualPixelMethod);
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_CONSTANT", ConstantVirtualPixelMethod);  /* deprecated */
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_EDGE", EdgeVirtualPixelMethod);
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_MIRROR", MirrorVirtualPixelMethod);
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_TILE", TileVirtualPixelMethod);
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_TRANSPARENT", TransparentVirtualPixelMethod);
#if MagickLibVersion > 0x641
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_MASK", MaskVirtualPixelMethod);
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_BLACK", BlackVirtualPixelMethod);
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_GRAY", GrayVirtualPixelMethod);
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_WHITE", WhiteVirtualPixelMethod);
#endif
#if MagickLibVersion > 0x642
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_HORIZONTALTILE", HorizontalTileVirtualPixelMethod);
	IMAGICK_REGISTER_CONST_LONG("VIRTUALPIXELMETHOD_VERTICALTILE", VerticalTileVirtualPixelMethod);
#endif
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_UNDEFINED", UndefinedPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_ROTATE", RotatePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SHEAR", ShearPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_ROLL", RollPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_HUE", HuePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SATURATION", SaturationPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_BRIGHTNESS", BrightnessPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_GAMMA", GammaPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SPIFF", SpiffPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_DULL", DullPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_GRAYSCALE", GrayscalePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_QUANTIZE", QuantizePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_DESPECKLE", DespecklePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_REDUCENOISE", ReduceNoisePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_ADDNOISE", AddNoisePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SHARPEN", SharpenPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_BLUR", BlurPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_THRESHOLD", ThresholdPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_EDGEDETECT", EdgeDetectPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SPREAD", SpreadPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SOLARIZE", SolarizePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SHADE", ShadePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_RAISE", RaisePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SEGMENT", SegmentPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_SWIRL", SwirlPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_IMPLODE", ImplodePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_WAVE", WavePreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_OILPAINT", OilPaintPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_CHARCOALDRAWING", CharcoalDrawingPreview);
	IMAGICK_REGISTER_CONST_LONG("PREVIEW_JPEG", JPEGPreview);
	IMAGICK_REGISTER_CONST_LONG("RENDERINGINTENT_UNDEFINED", UndefinedIntent);
	IMAGICK_REGISTER_CONST_LONG("RENDERINGINTENT_SATURATION", SaturationIntent);
	IMAGICK_REGISTER_CONST_LONG("RENDERINGINTENT_PERCEPTUAL", PerceptualIntent);
	IMAGICK_REGISTER_CONST_LONG("RENDERINGINTENT_ABSOLUTE", AbsoluteIntent);
	IMAGICK_REGISTER_CONST_LONG("RENDERINGINTENT_RELATIVE", RelativeIntent);
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_UNDEFINED", UndefinedInterlace);
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_NO", NoInterlace);
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_LINE", LineInterlace);
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_PLANE", PlaneInterlace);
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_PARTITION", PartitionInterlace);
#if MagickLibVersion > 0x633
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_GIF", GIFInterlace);
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_JPEG", JPEGInterlace);
	IMAGICK_REGISTER_CONST_LONG("INTERLACE_PNG", PNGInterlace);
#endif
	IMAGICK_REGISTER_CONST_LONG("FILLRULE_UNDEFINED", UndefinedRule);
	IMAGICK_REGISTER_CONST_LONG("FILLRULE_EVENODD", EvenOddRule);
	IMAGICK_REGISTER_CONST_LONG("FILLRULE_NONZERO", NonZeroRule);
	IMAGICK_REGISTER_CONST_LONG("PATHUNITS_UNDEFINED", UndefinedPathUnits);
	IMAGICK_REGISTER_CONST_LONG("PATHUNITS_USERSPACE", UserSpace);
	IMAGICK_REGISTER_CONST_LONG("PATHUNITS_USERSPACEONUSE", UserSpaceOnUse);
	IMAGICK_REGISTER_CONST_LONG("PATHUNITS_OBJECTBOUNDINGBOX", ObjectBoundingBox);
	IMAGICK_REGISTER_CONST_LONG("LINECAP_UNDEFINED", UndefinedCap);
	IMAGICK_REGISTER_CONST_LONG("LINECAP_BUTT", ButtCap);
	IMAGICK_REGISTER_CONST_LONG("LINECAP_ROUND", RoundCap);
	IMAGICK_REGISTER_CONST_LONG("LINECAP_SQUARE", SquareCap);
	IMAGICK_REGISTER_CONST_LONG("LINEJOIN_UNDEFINED", UndefinedJoin);
	IMAGICK_REGISTER_CONST_LONG("LINEJOIN_MITER", MiterJoin);
	IMAGICK_REGISTER_CONST_LONG("LINEJOIN_ROUND", RoundJoin);
	IMAGICK_REGISTER_CONST_LONG("LINEJOIN_BEVEL", BevelJoin);
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_UNDEFINED", UndefinedResource);
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_AREA", AreaResource);
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_DISK", DiskResource);
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_FILE", FileResource);
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_MAP", MapResource);
	IMAGICK_REGISTER_CONST_LONG("RESOURCETYPE_MEMORY", MemoryResource);
	IMAGICK_REGISTER_CONST_LONG("DISPOSE_UNRECOGNIZED", UnrecognizedDispose);
	IMAGICK_REGISTER_CONST_LONG("DISPOSE_UNDEFINED", UndefinedDispose);
	IMAGICK_REGISTER_CONST_LONG("DISPOSE_NONE", NoneDispose);
	IMAGICK_REGISTER_CONST_LONG("DISPOSE_BACKGROUND", BackgroundDispose);
	IMAGICK_REGISTER_CONST_LONG("DISPOSE_PREVIOUS", PreviousDispose);
#if MagickLibVersion > 0x631
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_UNDEFINED", UndefinedInterpolatePixel);
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_AVERAGE", AverageInterpolatePixel);
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_BICUBIC", BicubicInterpolatePixel);
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_BILINEAR", BilinearInterpolatePixel);
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_FILTER", FilterInterpolatePixel);
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_INTEGER", IntegerInterpolatePixel);
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_MESH", MeshInterpolatePixel);
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_NEARESTNEIGHBOR", NearestNeighborInterpolatePixel);
#endif
#if MagickLibVersion > 0x634
	IMAGICK_REGISTER_CONST_LONG("INTERPOLATE_SPLINE", SplineInterpolatePixel);
#endif
#if MagickLibVersion > 0x628
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_UNDEFINED", UndefinedLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_COALESCE", CoalesceLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_COMPAREANY", CompareAnyLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_COMPARECLEAR", CompareClearLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_COMPAREOVERLAY", CompareOverlayLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_DISPOSE", DisposeLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_OPTIMIZE", OptimizeLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_OPTIMIZEPLUS", OptimizePlusLayer);
#endif
#if MagickLibVersion > 0x629
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_OPTIMIZEIMAGE", OptimizeImageLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_OPTIMIZETRANS", OptimizeTransLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_REMOVEDUPS", RemoveDupsLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_REMOVEZERO", RemoveZeroLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_COMPOSITE", CompositeLayer);
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_UNDEFINED", UndefinedOrientation);
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_TOPLEFT", TopLeftOrientation);
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_TOPRIGHT", TopRightOrientation);
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_BOTTOMRIGHT", BottomRightOrientation);
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_BOTTOMLEFT", BottomLeftOrientation);
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_LEFTTOP", LeftTopOrientation);
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_RIGHTTOP", RightTopOrientation);
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_RIGHTBOTTOM", RightBottomOrientation);
	IMAGICK_REGISTER_CONST_LONG("ORIENTATION_LEFTBOTTOM", LeftBottomOrientation);
#endif
#if MagickLibVersion > 0x635
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_UNDEFINED", UndefinedDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_AFFINE", AffineDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_AFFINEPROJECTION", AffineProjectionDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_ARC", ArcDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_BILINEAR", BilinearDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_PERSPECTIVE", PerspectiveDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_PERSPECTIVEPROJECTION", PerspectiveProjectionDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_SCALEROTATETRANSLATE", ScaleRotateTranslateDistortion);
#endif
#if MagickLibVersion > 0x645
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_POLYNOMIAL", PolynomialDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_POLAR", PolarDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_DEPOLAR", DePolarDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_BARREL", BarrelDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_BARRELINVERSE", BarrelInverseDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_SHEPARDS", ShepardsDistortion);
	IMAGICK_REGISTER_CONST_LONG("DISTORTION_SENTINEL", SentinelDistortion);
#endif
#if MagickLibVersion > 0x636
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_MERGE", MergeLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_FLATTEN", FlattenLayer);
	IMAGICK_REGISTER_CONST_LONG("LAYERMETHOD_MOSAIC", MosaicLayer);
#endif
#if MagickLibVersion > 0x637
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_ACTIVATE", ActivateAlphaChannel);
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_DEACTIVATE", DeactivateAlphaChannel);
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_RESET", ResetAlphaChannel);
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_SET", SetAlphaChannel);
#endif
#if MagickLibVersion > 0x645
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_UNDEFINED", UndefinedAlphaChannel);
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_COPY", CopyAlphaChannel);
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_DEACTIVATE", DeactivateAlphaChannel);
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_EXTRACT", ExtractAlphaChannel);
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_OPAQUE", OpaqueAlphaChannel);
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_SHAPE", ShapeAlphaChannel);
	IMAGICK_REGISTER_CONST_LONG("ALPHACHANNEL_TRANSPARENT", TransparentAlphaChannel);
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_UNDEFINED", UndefinedColorInterpolate);
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_BARYCENTRIC", BarycentricColorInterpolate);
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_BILINEAR", BilinearColorInterpolate);
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_POLYNOMIAL", PolynomialColorInterpolate);
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_SPEPARDS", ShepardsColorInterpolate);
	IMAGICK_REGISTER_CONST_LONG("SPARSECOLORMETHOD_VORONOI", VoronoiColorInterpolate);
	IMAGICK_REGISTER_CONST_LONG("DITHERMETHOD_UNDEFINED", UndefinedDitherMethod);
	IMAGICK_REGISTER_CONST_LONG("DITHERMETHOD_NO", NoDitherMethod);
	IMAGICK_REGISTER_CONST_LONG("DITHERMETHOD_RIEMERSMA", RiemersmaDitherMethod);
	IMAGICK_REGISTER_CONST_LONG("DITHERMETHOD_FLOYDSTEINBERG", FloydSteinbergDitherMethod);
#endif
#if MagickLibVersion > 0x648
	IMAGICK_REGISTER_CONST_LONG("FUNCTION_UNDEFINED", UndefinedFunction);
	IMAGICK_REGISTER_CONST_LONG("FUNCTION_POLYNOMIAL", PolynomialFunction);
	IMAGICK_REGISTER_CONST_LONG("FUNCTION_SINUSOID", SinusoidFunction);
#endif
}
