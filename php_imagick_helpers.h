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
#ifndef PHP_IMAGICK_HELPERS_H 
# define PHP_IMAGICK_HELPERS_H

void initialize_imagick_constants();

void add_assoc_string_helper(zval *retvalue, char *name, char *key, char *hash_value TSRMLS_DC);

void *get_pointinfo_array(zval *coordinate_array, int *num_elements TSRMLS_DC);

#if MagickLibVersion <= 0x628
void count_pixeliterator_rows(php_imagickpixeliterator_object *internpix TSRMLS_DC );
#endif

int count_occurences_of(char needle, char *haystack TSRMLS_DC);

int check_configured_font(char *font, int font_len TSRMLS_DC);

int check_write_access(char *absolute TSRMLS_DC);

zend_bool php_imagick_thumbnail_dimensions(MagickWand *magick_wand, zend_bool bestfit, long desired_width, long desired_height, long *new_width, long *new_height);

zend_bool php_imagick_resize_bounding_box(MagickWand *magick_wand, long box_width, long box_height, zend_bool fill);

zend_bool crop_thumbnail_image(MagickWand *magick_wand, long desired_width, long desired_height TSRMLS_DC);

char *get_pseudo_filename(char* pseudo_string TSRMLS_DC);

double *get_double_array_from_zval(zval *param_array, long *num_elements TSRMLS_DC);

long *get_long_array_from_zval(zval *param_array, long *num_elements TSRMLS_DC);

unsigned char *get_char_array_from_zval(zval *param_array, long *num_elements TSRMLS_DC);

MagickBooleanType php_imagick_progress_monitor(const char *text, const MagickOffsetType offset, const MagickSizeType span, void *client_data);

zend_bool php_imagick_validate_map(const char *map TSRMLS_DC);


#endif /* PHP_IMAGICK_HELPERS_H */