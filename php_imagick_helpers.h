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

void php_imagick_initialize_constants(TSRMLS_D);

zend_bool php_imagick_check_font(char *font, int font_len TSRMLS_DC);

zend_bool php_imagick_thumbnail_dimensions(MagickWand *magick_wand, zend_bool bestfit, im_long desired_width, im_long desired_height, im_long *new_width, im_long *new_height, zend_bool legacy);

PointInfo *php_imagick_zval_to_pointinfo_array(zval *coordinate_array, int *num_elements TSRMLS_DC);

double *php_imagick_zval_to_double_array(zval *param_array, im_long *num_elements TSRMLS_DC);

long *php_imagick_zval_to_long_array(zval *param_array, long *num_elements TSRMLS_DC);

unsigned char *php_imagick_zval_to_char_array(zval *param_array, long *num_elements TSRMLS_DC);

MagickBooleanType php_imagick_progress_monitor(const char *text, const MagickOffsetType offset, const MagickSizeType span, void *client_data);

void php_imagick_cleanup_progress_callback(php_imagick_callback* progress_callback TSRMLS_DC);

MagickBooleanType php_imagick_progress_monitor_callable(const char *text, const MagickOffsetType offset, const MagickSizeType span, void *client_data);

zend_bool php_imagick_validate_map(const char *map TSRMLS_DC);

php_imagick_rw_result_t php_imagick_file_access_check (const char *filename TSRMLS_DC);


PixelWand *php_imagick_zval_to_pixelwand (zval *param, php_imagick_class_type_t caller, zend_bool *allocated TSRMLS_DC);
PixelWand *php_imagick_zval_to_opacity (zval *param, php_imagick_class_type_t caller, zend_bool *allocated TSRMLS_DC);


char *php_imagick_set_locale (TSRMLS_D);
void php_imagick_restore_locale (const char *old_locale);

PixelWand *php_imagick_clone_pixelwand (PixelWand *source);

void php_imagick_replace_magickwand (php_imagick_object *obj, MagickWand *new_wand);

void php_imagick_replace_drawingwand (php_imagickdraw_object *obj, DrawingWand *new_wand);

void php_imagick_replace_pixelwand (php_imagickpixel_object *obj, PixelWand *new_wand);

zend_bool php_imagick_ensure_not_empty (MagickWand *magick_wand);

/* Defined in imagickpixeliterator_class.c */
void php_imagick_pixel_iterator_new (PixelIterator *pixel_it, zval *return_value TSRMLS_DC);

/*
  Convert ImageMagick exception types to PHP exceptions
*/
void php_imagick_convert_imagick_exception (MagickWand *magick_wand, const char *default_message TSRMLS_DC);
void php_imagick_convert_imagickdraw_exception (DrawingWand *drawing_wand, const char *default_message TSRMLS_DC);
void php_imagick_convert_imagickpixeliterator_exception (PixelIterator *pixel_iterator, const char *default_message TSRMLS_DC);
void php_imagick_convert_imagickpixel_exception (PixelWand *pixel_wand, const char *default_message TSRMLS_DC);

/*
  Convert read or write failure to PHP exception
*/
void php_imagick_rw_fail_to_exception (MagickWand *magick_wand, php_imagick_rw_result_t rc, const char *filename TSRMLS_DC);
void php_imagick_imagickdraw_rw_fail_to_exception (DrawingWand *drawing_wand, php_imagick_rw_result_t rc, const char *filename TSRMLS_DC);

/*
  Throw an exception with message
*/
void php_imagick_throw_exception (php_imagick_class_type_t type, const char *description TSRMLS_DC);

#endif /* PHP_IMAGICK_HELPERS_H */