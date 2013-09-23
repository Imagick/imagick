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
#include "php_imagick_defs.h"
#include "php_imagick_macros.h"
#include "php_imagick_helpers.h"
#include "php_imagick_file.h"


#if MagickLibVersion > 0x628
/* {{{ proto bool Imagick::pingImageFile(resource filehandle)
    Query image information without reading the whole image to memory
*/
PHP_METHOD(imagick, pingimagefile)
{
	char *filename = NULL;
	int filename_len;
	php_imagick_object *intern;
	zval *zstream;
	php_stream *stream;
	zend_bool result;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|s!", &zstream, &filename, &filename_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	php_stream_from_zval(stream, &zstream);
	result = php_imagick_stream_handler(intern, stream, ImagickPingImageFile TSRMLS_CC);

	if (result == 0) {
		if (!EG(exception)) {
			php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to ping image from the filehandle" TSRMLS_CC);
		}
		return;
	}

	if (filename) {
		MagickSetImageFilename(intern->magick_wand, filename);
		MagickSetLastIterator(intern->magick_wand);
	}
	RETURN_TRUE;
}
/* }}} */


/* {{{ proto bool Imagick::pingImageBlob(string image )
	Query image information without reading the whole image to memory
*/
PHP_METHOD(imagick, pingimageblob)
{
	char *image_string;
	int image_string_len;
	MagickBooleanType status;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &image_string, &image_string_len) == FAILURE) {
		return;
	}

	if (image_string_len == 0) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Empty image string passed" TSRMLS_CC);
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickPingImageBlob(intern->magick_wand, image_string, image_string_len);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to ping image blob" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::vignetteImage(float blackPoint, float whitePoint, int x, int y )
	Adds vignette filter to the image
*/
PHP_METHOD(imagick, vignetteimage)
{
	double black_point, white_point;
	long x, y;
	php_imagick_object *intern;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddll", &black_point, &white_point, &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickVignetteImage(intern->magick_wand, black_point, white_point, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to apply vignette filter" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::transposeImage()
	Creates a vertical mirror image
*/
PHP_METHOD(imagick, transposeimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickTransposeImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to transpose image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::transverseImage()
	Creates a horizontal mirror image
*/
PHP_METHOD(imagick, transverseimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickTransverseImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to transverse image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::adaptiveBlurImage(float radius, float sigma[, int channel] )
	Adds adaptive blur filter to image
*/
PHP_METHOD(imagick, adaptiveblurimage)
{
	double radius, sigma;
	php_imagick_object *intern;
	MagickBooleanType status;
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|l", &radius, &sigma, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickAdaptiveBlurImageChannel(intern->magick_wand, channel, radius, sigma);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to adaptive blur image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::uniqueImageColors()
	Discards all but one of any pixel color
*/
PHP_METHOD(imagick, uniqueimagecolors)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickUniqueImageColors(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get unique image colors" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::contrastStretchImage(float black_point, float white_point[, int channel])
	Enhances the contrast of a color image
*/
PHP_METHOD(imagick, contraststretchimage)
{
	php_imagick_object *intern;
	double black_point, white_point;
	MagickBooleanType status;
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|l", &black_point, &white_point, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickContrastStretchImageChannel(intern->magick_wand, channel, black_point, white_point);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to contrast strech image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */


/* {{{ proto int Imagick::getImageMatte()
	Returns true if the image has a matte channel otherwise false
*/
PHP_METHOD(imagick, getimagematte)
{
	php_imagick_object *intern;
	MagickBooleanType matte;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	matte = MagickGetImageMatte(intern->magick_wand);

	if (matte == MagickTrue) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto bool Imagick::setImageMatte(bool matte)
	Sets the image matte channel
*/
PHP_METHOD(imagick, setimagematte)
{
	php_imagick_object *intern;
	zend_bool matte;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &matte) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageMatte(intern->magick_wand, matte);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image matte" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::adaptiveResizeImage(int width, int height[, bool bestfit])
	Adaptively resize image with data dependent triangulation
*/
PHP_METHOD(imagick, adaptiveresizeimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long width, height, new_width, new_height;
	zend_bool bestfit = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|b", &width, &height, &bestfit) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	if (!php_imagick_thumbnail_dimensions(intern->magick_wand, bestfit, width, height, &new_width, &new_height)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Invalid image geometry" TSRMLS_CC);
		return;
	}

	status = MagickAdaptiveResizeImage(intern->magick_wand, new_width, new_height);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to adaptive resize image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */
/* {{{ proto bool Imagick::sketchImage(float radius, float sigma, float angle)
	Simulates a pencil sketch
*/
PHP_METHOD(imagick, sketchimage)
{
	double radius, sigma, angle;
	MagickBooleanType status;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd", &radius, &sigma, &angle) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSketchImage(intern->magick_wand, sigma, radius, angle);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to sketch image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::shadeImage(bool gray, float azimuth, float elevation)
	Shines a distant light on an image
*/
PHP_METHOD(imagick, shadeimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double azimuth, elevation;
	zend_bool gray;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "bdd", &gray, &azimuth, &elevation) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickShadeImage(intern->magick_wand, gray, azimuth, elevation);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to shade image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto int Imagick::getSizeOffset()
	Returns the size offset associated with the Imagick object
*/
PHP_METHOD(imagick, getsizeoffset)
{
	php_imagick_object *intern;
	long offset;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickGetSizeOffset(intern->magick_wand, &offset);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get size offset" TSRMLS_CC);
		return;
	}

	RETVAL_LONG(offset);
}
/* }}} */

/* {{{ proto bool Imagick::setSizeOffset(int columns, int rows, int offset)
	Sets the size and offset of the Imagick object
*/
PHP_METHOD(imagick, setsizeoffset)
{
	php_imagick_object *intern;
	long columns, rows, offset;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll", &columns, &rows, &offset) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetSizeOffset(intern->magick_wand, columns, rows, offset);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set size offset" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::adaptiveSharpenImage(float radius, float sigma[, int channel])
	Adaptively sharpen image with data dependent triangulation
*/
PHP_METHOD(imagick, adaptivesharpenimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double radius, sigma;
	long channel = DefaultChannels;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|l", &radius, &sigma, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickAdaptiveSharpenImageChannel(intern->magick_wand, channel, radius, sigma);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to adaptive sharpen image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */


/* {{{ proto bool Imagick::randomThresholdImage(float low, float high[, int channel])
	Changes the value of individual pixels
*/
PHP_METHOD(imagick, randomthresholdimage)
{
	php_imagick_object *intern;
	double low, high;
	MagickBooleanType status;
	long channel = DefaultChannels;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|l", &low, &high, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickRandomThresholdImageChannel(intern->magick_wand, channel, low, high);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to random threshold image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string Imagick::roundCornersImage(float x_rounding, float y_rounding[, float stroke_width, float displace, float size_correction] )
   Rounds image corners
*/
PHP_METHOD(imagick, roundcornersimage)
{
	double x_rounding, y_rounding;
	DrawingWand *draw;
	MagickWand *mask_image;
	PixelWand *color;
	php_imagick_object *intern;
	long image_width, image_height;
	MagickBooleanType status;
	double stroke_width = 10, displace = 5, correction = -6;
	char *old_locale = NULL, *buffer = NULL;
	zend_bool restore = 0;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|ddd", &x_rounding, &y_rounding, &stroke_width, &displace, &correction) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	image_width = MagickGetImageWidth(intern->magick_wand);
	image_height = MagickGetImageHeight(intern->magick_wand);

	if (!image_width || !image_height) {
	    php_imagick_throw_exception(IMAGICK_CLASS, "Unable to round corners on empty image" TSRMLS_CC);
		return;
	}

	status = MagickSetImageMatte(intern->magick_wand, MagickTrue);

	if (status == MagickFalse) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Unable to set image matte" TSRMLS_CC);
		return;
	}

	/* Here we go.. */
	color      = (PixelWand *)NewPixelWand();
	draw       = (DrawingWand *)NewDrawingWand();
	mask_image = (MagickWand *)NewMagickWand();

#define exit_cleanup() \
	if (color != NULL) color = DestroyPixelWand(color); \
	if (draw != NULL) draw = DestroyDrawingWand(draw); \
	if (mask_image != NULL) mask_image = DestroyMagickWand(mask_image);

	status = PixelSetColor(color, "transparent");

	if (status == MagickFalse) {
        exit_cleanup();
		php_imagick_throw_exception(IMAGICK_CLASS, "Unable to set pixel color" TSRMLS_CC);
		return;
	}

	status = MagickNewImage(mask_image, image_width, image_height, color);

	if (status == MagickFalse) {
		exit_cleanup();
		php_imagick_throw_exception(IMAGICK_CLASS, "Unable to allocate mask image" TSRMLS_CC);
		return;
	}

	MagickSetImageBackgroundColor(mask_image, color);
	status = PixelSetColor(color, "white");

	if (status == MagickFalse) {
		exit_cleanup();
		php_imagick_throw_exception(IMAGICK_CLASS, "Unable to set pixel color" TSRMLS_CC);
		return;
	}

	DrawSetFillColor(draw, color);
	status = PixelSetColor(color, "black");

	if (status == MagickFalse) {
		exit_cleanup();
		php_imagick_throw_exception(IMAGICK_CLASS, "Unable to set pixel color" TSRMLS_CC);
		return;
	}

	DrawSetStrokeColor(draw, color);
	DrawSetStrokeWidth(draw, stroke_width);
	DrawRoundRectangle(draw, displace, displace, image_width + correction, image_height + correction, x_rounding, y_rounding);

	IMAGICK_SET_LOCALE(old_locale, buffer, restore);
	status = MagickDrawImage(mask_image, draw);
	IMAGICK_RESTORE_LOCALE(old_locale, restore);

	if (status == MagickFalse) {
		exit_cleanup();
		php_imagick_throw_exception(IMAGICK_CLASS, "Unable to draw on image" TSRMLS_CC);
		return;
	}

	status = MagickCompositeImage(intern->magick_wand, mask_image, DstInCompositeOp, 0, 0);

	if (status == MagickFalse) {
		exit_cleanup();
		php_imagick_throw_exception(IMAGICK_CLASS, "Unable to composite image" TSRMLS_CC);
		return;
	}

	exit_cleanup();
	RETURN_TRUE;
#undef exit_cleanup
}
/* }}} */

/* {{{ proto int Imagick::getIteratorIndex()
	Returns the index of the current active image
*/
PHP_METHOD(imagick, getiteratorindex)
{
	MagickBooleanType status;
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	status = MagickGetIteratorIndex(intern->magick_wand);
	ZVAL_LONG(return_value, (long)status);
	return;
}
/* }}} */

/* {{{ proto bool Imagick::setIteratorIndex(int index)
	Sets the index of the Imagick object
*/
PHP_METHOD(imagick, setiteratorindex)
{
	const long index;
	MagickBooleanType status;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetIteratorIndex(intern->magick_wand, index);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set iterator index" TSRMLS_CC);
		return;
	}

	intern->next_out_of_bound = 0;
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::transformimage(string crop, string geometry )
	Comfortability method for crop and resize
*/
PHP_METHOD(imagick, transformimage)
{
	char *crop, *geometry;
	int crop_len, geometry_len;
	MagickWand *transformed;
	php_imagick_object *intern, *intern_return;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &crop, &crop_len, &geometry, &geometry_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	transformed = MagickTransformImage(intern->magick_wand, crop, geometry);

	if (transformed == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Transforming image failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);

	IMAGICK_REPLACE_MAGICKWAND(intern_return, transformed);
	return;
}
/* }}} */
#endif

#if MagickLibVersion > 0x630
/* {{{ proto bool Imagick::setImageOpacity(float opacity)
	Sets the image to the specified opacity level
*/
PHP_METHOD(imagick, setimageopacity)
{
	double opacity;
	MagickBooleanType status;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &opacity) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageOpacity(intern->magick_wand, opacity);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image opacity" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::orderedposterizeImage(string threshold_map[, int CHANNEL])
	Performs an ordered dither
*/
PHP_METHOD(imagick, orderedposterizeimage)
{
	char *map;
	int map_len;
	MagickBooleanType status;
	php_imagick_object *intern;
	long channel = DefaultChannels;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &map, &map_len, &channel) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickOrderedPosterizeImageChannel(intern->magick_wand, channel, map);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to posterize image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x631
/* {{{ proto bool Imagick::polaroidImage(ImagickDraw properties, double angle )
	Simulates a Polaroid picture
*/
PHP_METHOD(imagick, polaroidimage)
{
	zval *objvar;
	php_imagick_object *intern;
	MagickBooleanType status;
	php_imagickdraw_object *internd;
	double angle;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Od", &objvar, php_imagickdraw_sc_entry, &angle) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);	

	internd = (php_imagickdraw_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	status = MagickPolaroidImage(intern->magick_wand, internd->drawing_wand, angle);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to polaroid image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string Imagick::getImageProperty(string name )
	Eeturns a value associated with the specified property
*/
PHP_METHOD(imagick, getimageproperty)
{
	php_imagick_object *intern;
	char *name, *value;
	int name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	value = MagickGetImageProperty(intern->magick_wand, name);

	if (value) {
		ZVAL_STRING(return_value, (char *)value, 1);
		IMAGICK_FREE_MEMORY(char *, value);
		return;
	}
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageProperty(string name, string value )
	returns a value associated with the specified property
*/
PHP_METHOD(imagick, setimageproperty)
{
	php_imagick_object *intern;
	char *name, *value;
	int name_len, value_len;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &name, &name_len, &value, &value_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	status = MagickSetImageProperty(intern->magick_wand, name, value);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image property" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto int Imagick::getImageInterpolateMethod()
	Returns the interpolation method for the sepcified image.
*/
PHP_METHOD(imagick, getimageinterpolatemethod)
{
	php_imagick_object *intern;
	long interpolate;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	interpolate = MagickGetImageInterpolateMethod(intern->magick_wand);
	RETVAL_LONG(interpolate);
}
/* }}} */

/* {{{ proto bool Imagick::setImageInterpolateMethod(int method)
	Sets the image interpolate pixel method.
*/
PHP_METHOD(imagick, setimageinterpolatemethod)
{
	php_imagick_object *intern;
	long interpolate;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &interpolate) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageInterpolateMethod(intern->magick_wand, interpolate);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set the image interpolate method" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::linearStretchImage(float blackPoint, float whitePoint)
	Stretches with saturation the image intensity.
*/
PHP_METHOD(imagick, linearstretchimage)
{
	php_imagick_object *intern;
	double blackPoint, whitePoint;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &blackPoint, &whitePoint) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickLinearStretchImage(intern->magick_wand, blackPoint, whitePoint);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to linear strech image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto int Imagick::getImageLength()
	returns the image length in bytes
*/
PHP_METHOD(imagick, getimagelength)
{
	php_imagick_object *intern;
	MagickSizeType length;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGetImageLength(intern->magick_wand, &length);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(IMAGICK_CLASS, "Unable to acquire image length" TSRMLS_CC);
		return;
	}

	RETVAL_LONG(length);
}
/* }}} */

/* {{{ proto bool Imagick::extentImage(int width, int height, int x, int y )
	Sets the image size
*/
PHP_METHOD(imagick, extentimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long width, height, x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &width, &height, &x, &y) == FAILURE)
	{
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickExtentImage(intern->magick_wand, width, height, x, y);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to extent image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x633
/* {{{ proto Imagick Imagick::getImageOrientation(void)
   Gets the image orientation
*/
PHP_METHOD(imagick, getimageorientation)
{
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	RETVAL_LONG(MagickGetImageOrientation(intern->magick_wand));
}
/* }}} */

/* {{{ proto Imagick Imagick::setImageOrientation(int orientation)
   Sets the image orientation
*/
PHP_METHOD(imagick, setimageorientation)
{
	php_imagick_object *intern;
	long orientation;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &orientation) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageOrientation(intern->magick_wand, orientation);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image orientation" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x634
/* {{{ proto Imagick Imagick::paintFloodfillImage(mixed fill, float fuzz, mixed bordercolor, int x, int y[, int channel])
   Sets the image orientation
*/
PHP_METHOD(imagick, paintfloodfillimage)
{
	php_imagick_object *intern;
	zval *fill_param, *border_param;
	long x, y, channel = DefaultChannels;
	double fuzz;
	MagickBooleanType status;
	PixelWand *fill_wand, *border_wand = NULL;
	zend_bool fill_allocated = 0, border_allocated = 0;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zdzll|l", &fill_param, &fuzz, &border_param, &x, &y, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	fill_wand = php_imagick_zval_to_pixelwand (fill_param, IMAGICK_CLASS, &fill_allocated TSRMLS_CC);
	if (!fill_wand)
		return;

	if (Z_TYPE_P(border_param) == IS_NULL) {
		status = MagickPaintFloodfillImage(intern->magick_wand, channel, fill_wand, fuzz, NULL, x, y);
	} else {
		border_wand = php_imagick_zval_to_pixelwand (border_param, IMAGICK_CLASS, &border_allocated TSRMLS_CC);
		if (!border_wand) {
			if (fill_allocated)
				DestroyPixelWand (fill_wand);
			return;
		}
		status = MagickPaintFloodfillImage(intern->magick_wand, channel, fill_wand, fuzz, border_wand, x, y);
	}

	if (fill_allocated)
		DestroyPixelWand (fill_wand);

	if (border_wand)
		DestroyPixelWand (border_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to paint floodfill image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x635
/* {{{ proto Imagick Imagick::clutImage(Imagick lookup[, int channel] )
   Replaces colors in the image from a color lookup table
*/
PHP_METHOD(imagick, clutimage)
{
	zval *objvar;
	php_imagick_object *intern, *lookup;
	MagickBooleanType status;
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|d", &objvar, php_imagick_sc_entry, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	lookup = (php_imagick_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(lookup->magick_wand);

	status = MagickClutImageChannel(intern->magick_wand, channel, lookup->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to replace colors in the image from a color lookup table" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::getImageProperties([string pattern, bool values] )
  	Returns all the property names that match the specified pattern
*/
PHP_METHOD(imagick, getimageproperties)
{
	zend_bool values = 1;
	char *pattern = "*", **properties, *property;
	int pattern_len;
	unsigned long properties_count, i;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sb", &pattern, &pattern_len, &values) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	properties = MagickGetImageProperties(intern->magick_wand, pattern, &properties_count);

	if (!properties) {
		php_imagick_convert_imagick_exception(IMAGICK_CLASS, "Unable to get image properties" TSRMLS_CC);
		return;
	}

	array_init(return_value);

	if (values) {

		for (i = 0; i < properties_count; i++) {
			property = MagickGetImageProperty(intern->magick_wand, properties[i]);
			add_assoc_string(return_value, properties[i], property, 1);
			IMAGICK_FREE_MEMORY(char *, property);
		}

	} else {

		for (i = 0; i < properties_count; i++) {
			add_next_index_string(return_value, properties[i], 1);
		}
	}

	IMAGICK_FREE_MEMORY(char **, properties);
	return;
}
/* }}} */

/* {{{ proto Imagick Imagick::getImageProfiles([string pattern, bool values] )
  	Returns all the profile names that match the specified pattern
*/
PHP_METHOD(imagick, getimageprofiles)
{
	zend_bool values = 1;
	char *pattern = "*", **profiles, *profile;
	int pattern_len;
	unsigned long profiles_count, i;
	php_imagick_object *intern;
	size_t length;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sb", &pattern, &pattern_len, &values) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	profiles = MagickGetImageProfiles(intern->magick_wand, pattern, &profiles_count);

	if (!profiles) {
		php_imagick_convert_imagick_exception(IMAGICK_CLASS, "Unable to get image profiles" TSRMLS_CC);
		return;
	}

	array_init(return_value);

	if (values) {

		for (i = 0; i < profiles_count; i++) {
			profile = (char *)MagickGetImageProfile(intern->magick_wand, profiles[i], &length);
			add_assoc_stringl(return_value, profiles[i], profile, length, 1);
			IMAGICK_FREE_MEMORY(char *, profile);
		}

	} else {

		for (i = 0; i < profiles_count; i++) {
			add_next_index_string(return_value, profiles[i], 1);
		}
	}

	IMAGICK_FREE_MEMORY(char **, profiles);
	return;
}
/* }}} */
#endif

#if MagickLibVersion > 0x635
/* {{{ proto Imagick Imagick::distortImage(int distortMethod, array arguments, bool bestfit )
   Distorts an image using various distortion methods
*/
PHP_METHOD(imagick, distortimage)
{
	php_imagick_object *intern;
	double *arguments;
	long distort_method, elements;
	zend_bool bestfit;
	zval *arg_array;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lab", &distort_method, &arg_array, &bestfit) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	arguments = php_imagick_zval_to_double_array(arg_array, &elements TSRMLS_CC);

	if (!arguments) {
		php_imagick_throw_exception (IMAGICK_CLASS, "Can't read argument array" TSRMLS_CC);
		return;
	}

	status = MagickDistortImage(intern->magick_wand, distort_method, elements, arguments, bestfit);
	efree(arguments);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to distort the image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::writeImageFile(resource $handle)
	Writes image to an open filehandle
*/
PHP_METHOD(imagick, writeimagefile)
{
	php_imagick_object *intern;
	zval *zstream;
	php_stream *stream;
	zend_bool result;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zstream) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	php_stream_from_zval(stream, &zstream);
	result = php_imagick_stream_handler(intern, stream, ImagickWriteImageFile TSRMLS_CC);

	if (result == 0) {
		if (!EG(exception)) {
			php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to write image to the filehandle" TSRMLS_CC);
		return;
		}
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::writeImagesFile(resource $handle)
	Writes images to an open filehandle
*/
PHP_METHOD(imagick, writeimagesfile)
{
	php_imagick_object *intern;
	zval *zstream;
	php_stream *stream;
	zend_bool result;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zstream) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	php_stream_from_zval(stream, &zstream);
	result = php_imagick_stream_handler(intern, stream, ImagickWriteImagesFile TSRMLS_CC);

	if (result == 0) {
		if (!EG(exception)) {
			php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to write images to the filehandle" TSRMLS_CC);
		return;
		}
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::resetImagePage(string page)
	Resets the page canvas and position
*/
PHP_METHOD(imagick, resetimagepage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	char *page;
	int page_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &page, &page_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickResetImagePage(intern->magick_wand, page);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to reset image page" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::getImageClipMask()
	Gets image clip mask
*/
PHP_METHOD(imagick, getimageclipmask)
{
	MagickWand *tmp_wand;
	php_imagick_object *intern;
	php_imagick_object *intern_return;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	tmp_wand = MagickGetImageClipMask(intern->magick_wand);
	
	/* No magick is going to happen */
	if (tmp_wand == NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image clip mask" TSRMLS_CC);
		return;
	}	
	
	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);

	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);
	return;
}
/* }}} */

/* {{{ proto bool Imagick::setImageClipMask(Imagick clip_mask)
	Sets the image clip mask
*/
PHP_METHOD(imagick, setimageclipmask)
{
	php_imagick_object *intern;
	php_imagick_object *clip_mask;
	MagickBooleanType status;
	zval *objvar;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &objvar, php_imagick_sc_entry) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	clip_mask = (php_imagick_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(clip_mask->magick_wand);
	
	status = MagickSetImageClipMask(intern->magick_wand, clip_mask->magick_wand);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image clip mask" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::animateImages(string server_name)
	Animates an image or image sequence
*/
PHP_METHOD(imagick, animateimages)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	char *server_name;
	int server_name_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &server_name, &server_name_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	(void)MagickSetFirstIterator(intern->magick_wand);
	status = MagickAnimateImages(intern->magick_wand, server_name);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to animate images" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::recolorImage(array matrix)
	Translate, scale, shear, or rotate image colors
*/
PHP_METHOD(imagick, recolorimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long num_elements;
	zval *matrix;
	double *array;
	unsigned long order;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &matrix) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	array = php_imagick_zval_to_double_array(matrix, &num_elements TSRMLS_CC);
	
	if (!array) {
		php_imagick_throw_exception (IMAGICK_CLASS, "The map contains disallowed characters" TSRMLS_CC);
		return;
	}
	
	order = (unsigned long)sqrt(num_elements);
	
	if (pow((double)order, 2) != num_elements) {
		efree(array);
		php_imagick_throw_exception(IMAGICK_CLASS, "The color matrix must contain a square number of elements" TSRMLS_CC);
		return;
	}

	status = MagickRecolorImage(intern->magick_wand, order, array);
	efree(array);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to recolor image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x636
PHP_METHOD(imagick, setfont)
{
	php_imagick_object *intern;
	char *font, *absolute;
	int font_len;
	MagickBooleanType status;
	php_imagick_rw_result_t rc;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &font, &font_len) == FAILURE) {
		return;
	}

	/* Check that no empty string is passed */
	if (font_len == 0) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Can not set empty font" TSRMLS_CC);
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	/* And if it wasn't */
	if (!php_imagick_check_font(font, font_len TSRMLS_CC)) {

		if (!(absolute = expand_filepath(font, NULL TSRMLS_CC))) {
			php_imagick_throw_exception(IMAGICK_CLASS, "Unable to set font" TSRMLS_CC);
			return;
		}

		if ((rc = php_imagick_file_access_check (absolute TSRMLS_CC)) != IMAGICK_RW_OK) {
			php_imagick_rw_fail_to_exception (intern->magick_wand, rc, absolute TSRMLS_CC);
			efree(absolute);
			return;
		}
		status = MagickSetFont(intern->magick_wand, absolute);
		efree(absolute);
	} else {
		status = MagickSetFont(intern->magick_wand, font);
	}

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set font" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}

PHP_METHOD(imagick, getfont)
{
	char *font;
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	font   = MagickGetFont(intern->magick_wand);

	if (font) {
		ZVAL_STRING(return_value, font, 1);
		IMAGICK_FREE_MEMORY(char *, font);
		return;
	}
	RETURN_FALSE;
}

PHP_METHOD(imagick, setpointsize)
{
	php_imagick_object *intern;
	double point_size;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &point_size) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetPointsize(intern->magick_wand, point_size);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set point size" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}

PHP_METHOD(imagick, getpointsize)
{
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETVAL_DOUBLE(MagickGetPointsize(intern->magick_wand));
}

PHP_METHOD(imagick, mergeimagelayers)
{
	php_imagick_object *intern, *intern_return;
	long layer_method;
	MagickWand *merged;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &layer_method) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	/* Reset the iterator */
	(void)MagickSetFirstIterator(intern->magick_wand);

	merged = MagickMergeImageLayers(intern->magick_wand, layer_method);

	/* No magick is going to happen */
	if (!merged) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to merge image layers" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);

	IMAGICK_REPLACE_MAGICKWAND(intern_return, merged);
	return;
}
#endif

#if MagickLibVersion > 0x637
/* {{{ proto bool Imagick::floodfillPaintImage(mixed fill, float fuzz, mixed bordercolor, int x, int y, bool invert[, int CHANNEL])
	Changes the color value of any pixel that matches target
*/
PHP_METHOD(imagick, floodfillpaintimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	zval *fill_param, *border_param;
	long x, y, channel = DefaultChannels;
	zend_bool invert;
	double fuzz;
	PixelWand *fill_wand, *border_wand;
	zend_bool fill_allocated = 0, border_allocated = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zdzllb|l", 
		&fill_param, &fuzz, &border_param, &x, &y, &invert, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	fill_wand = php_imagick_zval_to_pixelwand (fill_param, IMAGICK_CLASS, &fill_allocated TSRMLS_CC);
	if (!fill_wand)
		return;

	border_wand = php_imagick_zval_to_pixelwand (border_param, IMAGICK_CLASS, &border_allocated TSRMLS_CC);
	if (!border_wand) {
		if (fill_allocated)
			fill_wand = DestroyPixelWand (fill_wand);

		return;
	}
	status = MagickFloodfillPaintImage(intern->magick_wand, channel, fill_wand, fuzz, border_wand, x, y, invert);

	if (fill_allocated)
		fill_wand = DestroyPixelWand (fill_wand);

	if (border_allocated)
		border_wand = DestroyPixelWand (border_wand);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception (intern->magick_wand, "Unable to floodfill paint image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::opaquePaintImage(mixed target, mixed fill, float fuzz, bool invert[, int CHANNEL])
	Changes any pixel that matches color with the color defined by fill.
*/
PHP_METHOD(imagick, opaquepaintimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	zval *fill_param, *target_param;
	zend_bool invert;
	double fuzz;
	long channel = DefaultChannels;
	PixelWand *fill_wand, *target_wand;
	zend_bool fill_allocated = 0, target_allocated = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzdb|l", &fill_param, &target_param, &fuzz, &invert, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	fill_wand = php_imagick_zval_to_pixelwand (fill_param, IMAGICK_CLASS, &fill_allocated TSRMLS_CC);
	if (!fill_wand)
		return;

	target_wand = php_imagick_zval_to_pixelwand (target_param, IMAGICK_CLASS, &target_allocated TSRMLS_CC);
	if (!target_wand) {
		if (fill_allocated)
			DestroyPixelWand (fill_wand);
		return;
	}

	status = MagickOpaquePaintImageChannel(intern->magick_wand, channel, fill_wand, target_wand, fuzz, invert);

	if (fill_allocated)
		DestroyPixelWand (fill_wand);

	if (target_allocated)
		DestroyPixelWand (target_wand);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to opaque paint image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::transparentPaintImage(mixed target, float alpha, float fuzz, bool invert)
	Changes any pixel that matches color with the color defined by fill
*/
PHP_METHOD(imagick, transparentpaintimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	zval *target_param;
	zend_bool invert;
	double fuzz, alpha;
	PixelWand *target_wand;
	zend_bool target_allocated;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zddb", &target_param, &alpha, &fuzz, &invert) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	target_wand = php_imagick_zval_to_pixelwand (target_param, IMAGICK_CLASS, &target_allocated TSRMLS_CC);
	if (!target_wand)
		return;

	status = MagickTransparentPaintImage(intern->magick_wand, target_wand, alpha, fuzz, invert);

	if (target_allocated)
		target_wand = DestroyPixelWand (target_wand);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to paint transparent image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::setImageAlphaChannel(int ALPHACHANNEL )
   Activates, deactivates, resets, or sets the alpha channel
*/
PHP_METHOD(imagick, setimagealphachannel)
{
	php_imagick_object *intern;
	long alpha_channel;
	MagickBooleanType status;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &alpha_channel) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickSetImageAlphaChannel(intern->magick_wand, alpha_channel);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image alpha channel" TSRMLS_CC);
		return;
	}
	
	RETURN_TRUE;
}
#endif

#if MagickLibVersion > 0x638
/* {{{ proto Imagick Imagick::liquidRescaleImage(int cols, int rows, float delta_x, float rigidity )
   Rescales image with seam carving
*/
PHP_METHOD(imagick, liquidrescaleimage)
{
	php_imagick_object *intern;
	long cols, rows;
	double delta_x, rigidity;
	MagickBooleanType status;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lldd", &cols, &rows, &delta_x, &rigidity) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickLiquidRescaleImage(intern->magick_wand, cols, rows, delta_x, rigidity);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to liquid rescale image" TSRMLS_CC);
		return;
	}
	
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::decipherImage(string passphrase)
	Converts cipher pixels to plain pixels
*/
PHP_METHOD(imagick, decipherimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	char *passphrase;
	int passphrase_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &passphrase, &passphrase_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickDecipherImage(intern->magick_wand, passphrase);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to decipher image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::encipherImage(string passphrase)
	Converts plain pixels to cipher pixels
*/
PHP_METHOD(imagick, encipherimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	char *passphrase;
	int passphrase_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &passphrase, &passphrase_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickEncipherImage(intern->magick_wand, passphrase);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to encipher image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x639
/* {{ proto bool Imagick::setGravity(int GRAVITY)
		Sets the gravity value
*/
PHP_METHOD(imagick, setgravity)
{
	php_imagick_object *intern;
	long gravity;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &gravity) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (MagickSetGravity(intern->magick_wand, gravity) == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set gravity" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{ proto int Imagick::getGravity()
		Gets the gravity value
*/
PHP_METHOD(imagick, getgravity)
{
	php_imagick_object *intern;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	RETVAL_LONG(MagickGetGravity(intern->magick_wand));
}
/* }}} */

/* {{ proto Imagick Imagick::getImageChannelRange(int CHANNEL)
		Gets the image channel's range
 */
PHP_METHOD(imagick, getimagechannelrange)
{
	php_imagick_object *intern;
	long channel;
	double maxima, minima;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &channel) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	if (MagickGetImageChannelRange(intern->magick_wand, channel, &minima, &maxima) == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get channel range" TSRMLS_CC);
		return;
	}
	
	array_init(return_value);
	
	add_assoc_double(return_value, "minima", minima);
	add_assoc_double(return_value, "maxima", maxima);
	
	return;
}
/* }}} */

/* {{{ proto int Imagick::getImageAlphaChannel()
	Gets the image alpha channel value
*/
PHP_METHOD(imagick, getimagealphachannel)
{
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	RETVAL_LONG(MagickGetImageAlphaChannel(intern->magick_wand));
}
/* }}} */
#endif

#if MagickLibVersion > 0x642
/* {{{ proto float Imagick::getImageChannelDistortions(Imagick reference, int METRICTYPE[, int CHANNEL])
	Compares one or more image channels of an image to a reconstructed image and returns the specified distortion metrics
*/
PHP_METHOD(imagick, getimagechanneldistortions)
{
	php_imagick_object *intern;
	php_imagick_object *intern_reference;
	zval *reference_param;
	double distortion;
	long metric, channel = DefaultChannels;
	MagickBooleanType status;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Ol|l", &reference_param, php_imagick_sc_entry, &metric, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	intern_reference = (php_imagick_object *)zend_object_store_get_object(reference_param TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern_reference->magick_wand);
	
	status = MagickGetImageChannelDistortion(intern->magick_wand, intern_reference->magick_wand, channel, metric, &distortion);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image channel distortion metrics" TSRMLS_CC);
		return;
	}
	
	RETVAL_DOUBLE(distortion);
}
/* }}} */
#endif

#if MagickLibVersion > 0x643
/* {{{ proto int Imagick::getImageGravity()
	Gets the image gravity
*/
PHP_METHOD(imagick, getimagegravity)
{
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	RETVAL_LONG(MagickGetImageGravity(intern->magick_wand));
}
/* }}} */

/* {{{ proto bool Imagick::setImageGravity(int GRAVITY)
	Sets the image gravity
*/
PHP_METHOD(imagick, setimagegravity)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long gravity;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &gravity) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickSetImageGravity(intern->magick_wand, gravity);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image gravity" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x645
/* {{ proto Imagick Imagick::importImagePixels(int x, int y, int width, int height, string map, int STORAGE, array pixels)
	Accepts pixel data and stores it in the image at the location you specify
 */
PHP_METHOD(imagick, importimagepixels) 
{
	double        *double_array;
	long          *long_array;
	unsigned char *char_array;
	
	php_imagick_object *intern;
	MagickBooleanType status;

	long storage, num_elements;
	long x, y, width, height;
	int map_len;
	char *map;
	zval *pixels;
	HashTable *array;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llllsla", &x, &y, &width, &height, &map, &map_len, &storage, &pixels) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);	
	
	if (x < 0 || y < 0) {
		php_imagick_throw_exception (IMAGICK_CLASS, "The coordinates must be non-negative" TSRMLS_CC);
		return;
	}
	
	if (width <= 0 || height <= 0) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The width and height must be greater than zero" TSRMLS_CC);
		return;
	}
	
	array = Z_ARRVAL_P(pixels);
	
	if (zend_hash_num_elements(array) != ((width * height) * map_len)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The map contains incorrect number of elements" TSRMLS_CC);
		return;
	} else {
		if (!php_imagick_validate_map(map TSRMLS_CC)) {
			php_imagick_throw_exception(IMAGICK_CLASS, "The map contains disallowed characters" TSRMLS_CC);
			return;
		}
	}
	
	switch (storage) {	
		case FloatPixel:
		case DoublePixel:
			/* Use doubles */
			storage = DoublePixel;
			double_array = (double *)php_imagick_zval_to_double_array(pixels, &num_elements TSRMLS_CC);
			if (!double_array) {
				php_imagick_throw_exception(IMAGICK_CLASS, "The map must contain only numeric values" TSRMLS_CC);
				return;
			}
			status = MagickImportImagePixels(intern->magick_wand, x, y, width, height, map, storage, double_array);
			efree(double_array);
		break;
		
		case ShortPixel:
		case IntegerPixel:
		case LongPixel:
			/* Use longs */
			storage = LongPixel;
			long_array = (long *)php_imagick_zval_to_long_array(pixels, &num_elements TSRMLS_CC);
			if (!long_array) {
				php_imagick_throw_exception(IMAGICK_CLASS, "The map must contain only numeric values" TSRMLS_CC);
				return;
			}
			status = MagickImportImagePixels(intern->magick_wand, x, y, width, height, map, storage, long_array);
			efree(long_array);
		break;
		
		case CharPixel:
			char_array = (unsigned char *)php_imagick_zval_to_char_array(pixels, &num_elements TSRMLS_CC);
			if (!char_array) {
				php_imagick_throw_exception(IMAGICK_CLASS, "The character array contains incorrect values" TSRMLS_CC);
				return;
			}
			status = MagickImportImagePixels(intern->magick_wand, x, y, width, height, map, storage, char_array);
			efree(char_array);
		break;
		
		default:
			php_imagick_throw_exception(IMAGICK_CLASS, "Unknown storage format" TSRMLS_CC);
			return;
		break;
	}
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to import image pixels" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

PHP_METHOD(imagick, deskewimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double threshold;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &threshold) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickDeskewImage(intern->magick_wand, threshold);
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to deskew image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}

PHP_METHOD(imagick, segmentimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long colorspace;
	zend_bool verbose = 0;
	double cluster_threshold, smooth_threshold;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ldd|b", &colorspace, &cluster_threshold, &smooth_threshold, &verbose) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickSegmentImage(intern->magick_wand, colorspace, verbose, cluster_threshold, smooth_threshold);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to segment image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}

PHP_METHOD(imagick, sparsecolorimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long num_elements, sparse_method, channel = DefaultChannels;
	zval *arguments;
	double *double_array;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "la|l", &sparse_method, &arguments, &channel) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	double_array = (double *)php_imagick_zval_to_double_array(arguments, &num_elements TSRMLS_CC);
	
	if (!double_array) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The map must contain only numeric values" TSRMLS_CC);
		return;
	}
	
	status = MagickSparseColorImage(intern->magick_wand, channel, sparse_method, num_elements, double_array);
	efree(double_array);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to sparse color image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;	
}

/* {{{ proto bool Imagick::remapImage(Imagick remap, int DITHERMETHOD)
	replaces the colors of an image with the closest color from a reference image
*/
PHP_METHOD(imagick, remapimage)
{
	php_imagick_object *intern;
	php_imagick_object *intern_remap;
	MagickBooleanType status;
	long dither_method;
	zval *remap_param;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Ol", &remap_param, php_imagick_sc_entry, &dither_method) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	intern_remap = (php_imagick_object *)zend_object_store_get_object(remap_param TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern_remap->magick_wand);

	status = MagickRemapImage(intern->magick_wand, intern_remap->magick_wand, dither_method);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to remap image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x646
/* {{{ proto bool Imagick::exportImagePixels(int x, int y, int width, int height, string map, INT STORAGE)
	TODO: IMPLEMENTATION
*/
PHP_METHOD(imagick, exportimagepixels)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long x, y, width, height, storage;
	char *map;
	int map_size, map_len, i = 0;
	double *double_array;
	long *long_array;
	char *char_array;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llllsl", &x, &y, &width, &height, &map, &map_len, &storage) == FAILURE) {
		return;
	}

	if ((x < 0) || (y < 0)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The coordinates must be non-negative" TSRMLS_CC);
		return;
	}

	if (width <= 0 || height <= 0) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The width and height must be greater than zero" TSRMLS_CC);
		return;
	}

	if (!php_imagick_validate_map(map TSRMLS_CC)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The map contains disallowed characters" TSRMLS_CC);
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	map_size = (map_len * width * height);

	switch (storage) {
		case FloatPixel:
		case DoublePixel:
			double_array = emalloc(map_size * sizeof(double));
			status = MagickExportImagePixels(intern->magick_wand, x, y, width, height, map, DoublePixel, (void *)double_array);

			if (status != MagickFalse) {
				array_init(return_value);
				for (i = 0; i < map_size; i++) {
					add_next_index_double(return_value, double_array[i]);
				}
				efree(double_array);
				return;
			}
		break;

		case ShortPixel:
		case IntegerPixel:
		case LongPixel:
			long_array = emalloc(map_size * sizeof(long));
			status = MagickExportImagePixels(intern->magick_wand, x, y, width, height, map, LongPixel, (void *)long_array);

			if (status != MagickFalse) {
				array_init(return_value);
				for (i = 0; i < map_size; i++) {
					add_next_index_long(return_value, long_array[i]);
				}
				efree(long_array);
				return;
			}
		break;

		case CharPixel:
			char_array = emalloc(map_size * sizeof(char));
			status = MagickExportImagePixels(intern->magick_wand, x, y, width, height, map, CharPixel, (void *)char_array);

			if (status != MagickFalse) {
				array_init(return_value);
				for (i = 0; i < map_size; i++) {
					 add_next_index_long(return_value, (int)char_array[i]);
				}
				efree(char_array);
				return;
			}
		break;

		default:
			php_imagick_throw_exception(IMAGICK_CLASS, "Unknown storage format" TSRMLS_CC);
			return;
		break;
	}

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to export image pixels" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x648
/* {{{ proto array Imagick::getImageChannelKurtosis([int CHANNEL])
	Gets the kurtosis and skewness of one or more image channels
*/
PHP_METHOD(imagick, getimagechannelkurtosis)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double kurtosis, skewness;
	long channel = DefaultChannels;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickGetImageChannelKurtosis(intern->magick_wand, channel, &kurtosis, &skewness);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image channel kurtosis" TSRMLS_CC);
		return;
	}

	array_init(return_value);
	add_assoc_double(return_value, "kurtosis", kurtosis);
	add_assoc_double(return_value, "skewness", skewness);
	return;
}
/* }}} */

/* {{{ proto bool Imagick::functionImage(int FUNCTION, array arguments[, int CHANNEL])
	Applies an arithmetic, relational, or logical expression to an image
*/
PHP_METHOD(imagick, functionimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	zval *arguments;
	long func, num_elements, channel = DefaultChannels;
	double *array;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "la|l", &func, &arguments, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	array = php_imagick_zval_to_double_array(arguments, &num_elements TSRMLS_CC);

	if (!array) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The arguments array contains disallowed characters" TSRMLS_CC);
		return;
	}

	status = MagickFunctionImageChannel(intern->magick_wand, channel, func, num_elements, array);
	efree(array);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to execute function on the image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x651
/* {{{ proto boolean Imagick::transformImageColorspace()
	Transform image colorspace
*/
PHP_METHOD(imagick, transformimagecolorspace)
{
	php_imagick_object *intern;
	long colorspace;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &colorspace) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickTransformImageColorspace(intern->magick_wand, colorspace);

    /* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to transform image colorspace" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x652
/* {{{ proto boolean Imagick::haldClutImage(Imagick hald[, int CHANNEL])
   Replaces colors in the image from a Hald color lookup table
*/
PHP_METHOD(imagick, haldclutimage)
{
	zval *objvar;
	MagickBooleanType status;
	php_imagick_object *intern, *hald;
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|l", &objvar, php_imagick_sc_entry, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	hald = (php_imagick_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(hald->magick_wand);

	status = MagickHaldClutImageChannel(intern->magick_wand, channel, hald->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to hald clut image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x656
PHP_METHOD(imagick, setimageartifact)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	char *artifact, *value;
	int artifact_len, value_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &artifact, &artifact_len, &value, &value_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageArtifact(intern->magick_wand, artifact, value);
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image artifact" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}


PHP_METHOD(imagick, getimageartifact) 
{
	php_imagick_object *intern;
	char *artifact, *value;
	int artifact_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &artifact, &artifact_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	value = MagickGetImageArtifact(intern->magick_wand, artifact);

	if (!value) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image artifact" TSRMLS_CC);
		return;
	}
	RETVAL_STRING(value, 1);
	IMAGICK_FREE_MEMORY(char *, value);
	return;
}

/* {{{ proto boolean Imagick::deleteImageArtifact(string $artifact)
   Delete image artifact
*/
PHP_METHOD(imagick, deleteimageartifact) 
{
	php_imagick_object *intern;
	MagickBooleanType status;
	char *artifact;
	int artifact_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &artifact, &artifact_len) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickDeleteImageArtifact(intern->magick_wand, artifact);
	
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to delete image artifact" TSRMLS_CC);
		return;
	}
	
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto integer Imagick::getColorspace()
   Get the object colorspace property
*/
PHP_METHOD(imagick, getcolorspace) 
{
	php_imagick_object *intern;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETURN_LONG(MagickGetColorspace(intern->magick_wand));
}
/* }}} */

/* {{{ proto boolean Imagick::setColorspace([int COLORSPACE])
   Set the object colorspace property
*/
PHP_METHOD(imagick, setcolorspace) 
{
	php_imagick_object *intern;
	long colorspace;
	MagickBooleanType status;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &colorspace) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetColorspace(intern->magick_wand, colorspace);
	
	RETURN_BOOL(status == MagickTrue);
}
/* }}} */
#endif

/* {{{ proto Imagick Imagick::__construct([mixed files] )
   The Imagick constructor
*/
PHP_METHOD(imagick, __construct)
{
	php_imagick_object *intern;
	zval *files = NULL;
	HashPosition pos;
	php_imagick_rw_result_t rc;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z!/", &files) == FAILURE) {
		return;
	}

	/* No files given.. or null passed */
	if (!files) {
		return;
	}
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (Z_TYPE_P (files) == IS_LONG || Z_TYPE_P (files) == IS_DOUBLE)
		convert_to_string (files);

	/* A single file was given */
	if (Z_TYPE_P(files) == IS_STRING) {
		struct php_imagick_file_t file = {0};

		if (!php_imagick_file_init(&file, Z_STRVAL_P(files), Z_STRLEN_P(files) TSRMLS_CC)) {
			php_imagick_throw_exception(IMAGICK_CLASS, "The filename is too long" TSRMLS_CC);
			return;
		}
		rc = php_imagick_read_file(intern, &file, ImagickReadImage TSRMLS_CC);
		php_imagick_file_deinit(&file);

		if (rc != IMAGICK_RW_OK) {
			php_imagick_rw_fail_to_exception (intern->magick_wand, rc, Z_STRVAL_P(files) TSRMLS_CC);
			return;
		}
	}

	/* an array of filenames was given */
	else
	if (Z_TYPE_P(files) == IS_ARRAY) {
		for(zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(files), &pos);
			zend_hash_has_more_elements_ex(Z_ARRVAL_P(files), &pos) == SUCCESS;
			zend_hash_move_forward_ex(Z_ARRVAL_P(files), &pos)) {

			struct php_imagick_file_t file = {0};
			zval **ppzval;

			if (zend_hash_get_current_data_ex(Z_ARRVAL_P(files), (void**)&ppzval, &pos) == FAILURE) {
				continue;
			}

			if (!php_imagick_file_init(&file, Z_STRVAL_PP(ppzval), Z_STRLEN_PP(ppzval) TSRMLS_CC)) {
				php_imagick_throw_exception(IMAGICK_CLASS, "The filename is too long" TSRMLS_CC);
				return;
			}

			rc = php_imagick_read_file(intern, &file, ImagickReadImage TSRMLS_CC);
			php_imagick_file_deinit(&file);

			if (rc != IMAGICK_RW_OK) {
				php_imagick_rw_fail_to_exception (intern->magick_wand, rc, Z_STRVAL_PP(ppzval) TSRMLS_CC);
				return;
			}
		}
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string Imagick::__toString()
   Returns the current image as string
*/
PHP_METHOD(imagick, __tostring)
{
	php_imagick_object *intern;
	unsigned char *image;
	char *buffer;
	size_t image_size;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (MagickGetNumberImages(intern->magick_wand) == 0) {
		ZVAL_STRING(return_value, "", 1);
		return;
	}

	buffer = MagickGetImageFormat(intern->magick_wand);

	if (!buffer) {
		ZVAL_STRING(return_value, "", 1);
		return;
	} else {
		IMAGICK_FREE_MEMORY(char *, buffer);
	}

	image = MagickGetImageBlob(intern->magick_wand, &image_size);
	ZVAL_STRINGL(return_value, (char *)image, image_size, 1);
	IMAGICK_FREE_MEMORY(unsigned char *, image);
	return;
}
/* }}} */

/* {{{ proto array Imagick::queryFormats([string pattern] )
   Returns formats supported by ImageMagick
*/
PHP_METHOD(imagick, queryformats)
{
	char **supported_formats;
	unsigned long num_formats = 0, i;
	char *pattern = "*";
	int pattern_len = 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &pattern, &pattern_len) == FAILURE) {
		return;
	}

	supported_formats = (char **) MagickQueryFormats(pattern, &num_formats);
	array_init(return_value);

	for (i = 0 ; i < num_formats ; i++) {
		add_next_index_string(return_value, supported_formats[i], 1);
		IMAGICK_FREE_MEMORY(char *, supported_formats[i]);
	}

	IMAGICK_FREE_MEMORY(char **, supported_formats);
	return;
}
/* }}} */

/* {{{ proto array Imagick::queryFonts([string pattern] )
   Returns fonts supported by ImageMagick
*/
PHP_METHOD(imagick, queryfonts)
{
	char **fonts;
	unsigned long num_fonts = 0, i;
	char *pattern = "*";
	int pattern_len = 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &pattern, &pattern_len) == FAILURE) {
		return;
	}

	fonts = (char **) MagickQueryFonts(pattern, &num_fonts);
	array_init(return_value);

	for (i = 0 ; i < num_fonts ; i++) {
		add_next_index_string(return_value, fonts[i], 1);
		IMAGICK_FREE_MEMORY(char *, fonts[i]);
	}

	IMAGICK_FREE_MEMORY(char **, fonts);
	return;
}
/* }}} */

/* {{{ proto array Imagick::queryFontMetrics(ImagickDraw draw, string text[, bool multiline] )
   Returns a 13 element array representing the font metrics
*/
PHP_METHOD(imagick, queryfontmetrics)
{
	zval *objvar, *multiline = NULL;
	zend_bool remove_canvas = 0, query_multiline;
	php_imagick_object *intern;
	php_imagickdraw_object *internd;
	char *text;
	int text_len;
	double *metrics;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Os|z!", &objvar, php_imagickdraw_sc_entry, &text, &text_len, &multiline) == FAILURE) {
		return;
	}

	/* No parameter passed, this means we should autodetect */
	if (!multiline) {
		if (strchr (text, '\n') != NULL) {
			query_multiline = 1;
		} else {
			query_multiline = 0;
		}
	} else {
		convert_to_boolean(multiline);
		query_multiline = Z_BVAL_P(multiline);
	}

	/* fetch the objects */
	intern  = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	internd = (php_imagickdraw_object *)zend_object_store_get_object(objvar TSRMLS_CC);

	/* If wand is empty, create a 1x1 pixel image to use as a temporary canvas */
	if (MagickGetNumberImages(intern->magick_wand) < 1) {
		PixelWand *pixel_wand;
		MagickBooleanType status;
		
		/* Empty bg color */
		pixel_wand = NewPixelWand();
		
		if (!pixel_wand) {
			php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to allocate background color for the temporary canvas" TSRMLS_CC);
			return;
		}
		
		/* 1x1 should be enough to get the metrics */
		status     = MagickNewImage(intern->magick_wand, 1, 1, pixel_wand);
		pixel_wand = DestroyPixelWand(pixel_wand);
		
		if (status == MagickFalse) {
			php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to allocate temporary canvas" TSRMLS_CC);
			return;
		}
		remove_canvas = 1;
	}

	/* Multiline testing */
	if (query_multiline) {
		metrics = MagickQueryMultilineFontMetrics(intern->magick_wand, internd->drawing_wand, text);
	} else {
		metrics = MagickQueryFontMetrics(intern->magick_wand, internd->drawing_wand, text);
	}

	/* Remove the image from the stack*/
	if (remove_canvas) {
		MagickRemoveImage(intern->magick_wand);
	}
	
	if (!metrics) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Failed to query the font metrics" TSRMLS_CC);
		return;
	} else {
		zval *bounding;

		array_init(return_value);
		add_assoc_double(return_value, "characterWidth", metrics[0]);
		add_assoc_double(return_value, "characterHeight", metrics[1]);
		add_assoc_double(return_value, "ascender", metrics[2]);
		add_assoc_double(return_value, "descender", metrics[3]);
		add_assoc_double(return_value, "textWidth", metrics[4]);
		add_assoc_double(return_value, "textHeight", metrics[5]);
		add_assoc_double(return_value, "maxHorizontalAdvance", metrics[6]);

		MAKE_STD_ZVAL(bounding);
		array_init(bounding);

		add_assoc_double(bounding, "x1", metrics[7]);
		add_assoc_double(bounding, "y1", metrics[8]);
		add_assoc_double(bounding, "x2", metrics[9]);
		add_assoc_double(bounding, "y2", metrics[10]);
		add_assoc_zval(return_value, "boundingBox", bounding);

		add_assoc_double(return_value, "originX", metrics[11]);
		add_assoc_double(return_value, "originY", metrics[12]);

		IMAGICK_FREE_MEMORY(double *, metrics);
		return;
	}
}
/* }}} */

/* {{{ proto bool Imagick::valid()
    Checks if the current item is valid
*/
PHP_METHOD(imagick, valid)
{
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (intern->next_out_of_bound > 0) {
		RETURN_FALSE;
	}

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::current()
    Returns reference to the current imagick object with image pointer at the correct sequence.
*/
PHP_METHOD(imagick, current)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */

/* {{{ proto bool Imagick::readImage(string filename)
    Reads image from filename
*/
PHP_METHOD(imagick, readimage)
{
	char *filename;
	int filename_len;
	php_imagick_object *intern;
	struct php_imagick_file_t file = {0};
	php_imagick_rw_result_t rc;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filename, &filename_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (!php_imagick_file_init(&file, filename, filename_len TSRMLS_CC)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The filename is too long" TSRMLS_CC);
		return;
	}

	rc = php_imagick_read_file(intern, &file, ImagickReadImage TSRMLS_CC);
	php_imagick_file_deinit(&file);

	if (rc != IMAGICK_RW_OK) {
		php_imagick_rw_fail_to_exception (intern->magick_wand, rc, filename TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::readImages(array files )
    Reads image from an array of filenames
*/
PHP_METHOD(imagick, readimages)
{
	zval *files;
	php_imagick_object *intern;
	HashPosition pos;
	php_imagick_rw_result_t rc;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &files) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	for(zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(files), &pos);
		zend_hash_has_more_elements_ex(Z_ARRVAL_P(files), &pos) == SUCCESS;
		zend_hash_move_forward_ex(Z_ARRVAL_P(files), &pos)) {

		struct php_imagick_file_t file = {0};
		zval **ppzval;

		if (zend_hash_get_current_data_ex(Z_ARRVAL_P(files), (void**)&ppzval, &pos) == FAILURE) {
			continue;
		}

		if (!php_imagick_file_init(&file, Z_STRVAL_PP(ppzval), Z_STRLEN_PP(ppzval) TSRMLS_CC)) {
			php_imagick_throw_exception(IMAGICK_CLASS, "The filename is too long" TSRMLS_CC);
			return;
		}

		rc = php_imagick_read_file(intern, &file, ImagickReadImage TSRMLS_CC);
		php_imagick_file_deinit(&file);

		if (rc != IMAGICK_RW_OK) {
			php_imagick_rw_fail_to_exception (intern->magick_wand, rc, Z_STRVAL_PP(ppzval) TSRMLS_CC);
			return;
		}
	}
	RETURN_TRUE;
}


/* {{{ proto bool Imagick::pingImage(string filename )
    This method can be used to query image width, height, size, and format without reading the whole image in to memory.
*/
PHP_METHOD(imagick, pingimage)
{
	char *filename;
	int filename_len;
	php_imagick_object *intern;
	struct php_imagick_file_t file = {0};
	php_imagick_rw_result_t rc;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filename, &filename_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (!php_imagick_file_init(&file, filename, filename_len TSRMLS_CC)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The filename is too long" TSRMLS_CC);
		return;
	}
	rc = php_imagick_read_file(intern, &file, ImagickPingImage TSRMLS_CC);
	php_imagick_file_deinit(&file);

	if (rc != IMAGICK_RW_OK) {
		php_imagick_rw_fail_to_exception (intern->magick_wand, rc, filename TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::readImageFile(resource filehandle )
    Reads image from open filehandle
*/
PHP_METHOD(imagick, readimagefile)
{
	char *filename = NULL;
	int filename_len;
	php_imagick_object *intern;
	zval *zstream;
	zend_bool result;
	php_stream *stream;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|s!", &zstream, &filename, &filename_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	php_stream_from_zval(stream, &zstream);
	result = php_imagick_stream_handler(intern, stream, ImagickReadImageFile TSRMLS_CC);

	if (result == 0) {
		if (!EG(exception)) {
			php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to read image from the filehandle" TSRMLS_CC);
			return;
		}
		return;
	}

	if (filename) {
		MagickSetImageFilename(intern->magick_wand, filename);
		MagickSetLastIterator(intern->magick_wand);
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::displayImage(string serverName )
	Displays an image
*/
PHP_METHOD(imagick, displayimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	char *server_name;
	int server_name_len;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &server_name, &server_name_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickDisplayImage(intern->magick_wand, server_name);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to display image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::displayImages(string serverName )
	displays an image or image sequence
*/
PHP_METHOD(imagick, displayimages)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	char *server_name;
	int server_name_len;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &server_name, &server_name_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	status = MagickDisplayImages(intern->magick_wand, server_name);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to display images" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::readBlob(string image )
    Reads image from a binary string
*/
PHP_METHOD(imagick, readimageblob)
{
	char *image_string;
	char *filename = NULL;
	long filename_len;
	int image_string_len;
	MagickBooleanType status;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s!", &image_string, &image_string_len, &filename, &filename_len) == FAILURE) {
		return;
	}

	if (!image_string_len) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Zero size image string passed" TSRMLS_CC);
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickReadImageBlob(intern->magick_wand, image_string, image_string_len);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to read image blob" TSRMLS_CC);
		return;
	}

	/* Even if filename is null we need to give a name here. Otherwise segfaults will happen */
	MagickSetImageFilename(intern->magick_wand, filename);
	MagickSetLastIterator(intern->magick_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::blurImage(float radius, float sigma[, int channel ] )
	Adds blur filter to image. Optional third parameter to blur a specific channel.
*/
PHP_METHOD(imagick, blurimage)
{
	double radius, sigma;
	php_imagick_object *intern;
	MagickBooleanType status;
	/* Initialize channel type to all channels*/
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|l", &radius, &sigma, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickBlurImageChannel(intern->magick_wand, channel, radius, sigma);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to blur image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::waveImage(float amplitude, float length )
	Adds wave filter to the image.
*/
PHP_METHOD(imagick, waveimage)
{
	double amplitude, wave_length;
	php_imagick_object *intern;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &amplitude, &wave_length) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickWaveImage(intern->magick_wand, amplitude, wave_length);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to wave image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::clear()
	Clears all resources associated to Imagick object
*/
PHP_METHOD(imagick, clear)
{
	php_imagick_object *intern;
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	if (intern->magick_wand == (MagickWand *)NULL) {
		RETURN_FALSE;
	}

	ClearMagickWand(intern->magick_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::destroy()
	Destroys the Imagick object
*/
PHP_METHOD(imagick, destroy)
{
	zval *object;
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	object = getThis();
	intern = (php_imagick_object *)zend_object_store_get_object(object TSRMLS_CC);

	if (intern->magick_wand == (MagickWand *)NULL) {
		RETURN_FALSE;
	}

	ClearMagickWand(intern->magick_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::scaleImage(int width, int height[, bool bestfit = false] )
	Scales the size of an image to the given dimensions. Passing zero as either of
	the arguments will preserve dimension while scaling.
*/
PHP_METHOD(imagick, scaleimage)
{
	long width, height, new_width, new_height;
	php_imagick_object *intern;
	MagickBooleanType status;
	zend_bool bestfit = 0;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|b", &width, &height, &bestfit) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	if (!php_imagick_thumbnail_dimensions(intern->magick_wand, bestfit, width, height, &new_width, &new_height)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Invalid image geometry" TSRMLS_CC);
		return;
	}

	status = MagickScaleImage(intern->magick_wand, new_width, new_height);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to scale image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::cropImage(int width, height, int x, int y )
	Extracts a region of the image.
*/
PHP_METHOD(imagick, cropimage)
{
	long width, height, x, y;
	php_imagick_object *intern;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &width, &height, &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickCropImage(intern->magick_wand, width, height, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to crop image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::spreadImage(float radius )
	Special effects method that randomly displaces each pixel in a block defined by the radius parameter.
*/
PHP_METHOD(imagick, spreadimage)
{
	double radius;
	php_imagick_object *intern;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &radius) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSpreadImage(intern->magick_wand, radius);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to spread image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::swirlImage(float degrees )
	Swirls the pixels about the center of the image, where degrees indicates the sweep of the arc through which each pixel is moved. You get a more dramatic effect as the degrees move from 1 to 360.
*/
PHP_METHOD(imagick, swirlimage)
{
	double degrees;
	php_imagick_object *intern;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &degrees) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSwirlImage(intern->magick_wand, degrees);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to swirl image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::stripImage()
	Strips an image of all profiles and comments.
*/
PHP_METHOD(imagick, stripimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickStripImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to strip image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::trimImage(double fuzz)
	Remove edges that are the background color from the image.
*/
PHP_METHOD(imagick, trimimage)
{
	double fuzz;
	php_imagick_object *intern;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &fuzz) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickTrimImage(intern->magick_wand, fuzz);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to trim image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::chopImage(int width, int height, int x, int y)
	Removes a region of an image and collapses the image to occupy the removed portion
*/
PHP_METHOD(imagick, chopimage)
{
	php_imagick_object *intern;
	long width, height, x, y;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &width, &height, &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickChopImage(intern->magick_wand, width, height, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to chop image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;

}
/* }}} */

/* {{{ proto bool Imagick::clipImage()
	Clips along the first path from the 8BIM profile, if present.
*/
PHP_METHOD(imagick, clipimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	status = MagickClipImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to clip image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::clipPathImage(string pathname, bool inside)
	Clips along the named paths from the 8BIM profile, if present. Later operations take effect inside the path.  Id may be a number if preceded with #, to work on a numbered path, e.g., "#1" to use the first path.
*/
PHP_METHOD(imagick, clippathimage)
{
	php_imagick_object *intern;
	char *clip_path;
	int clip_path_len;
	zend_bool inside;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sb", &clip_path, &clip_path_len, &inside) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
#if MagickLibVersion > 0x636
	status = MagickClipImagePath(intern->magick_wand, clip_path, inside);
#else
	status = MagickClipPathImage(intern->magick_wand, clip_path, inside);
#endif

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to clip path image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;

}
/* }}} */

/* {{{ proto Imagick Imagick::coalesceImages()
	Composites a set of images while respecting any page offsets and disposal methods.  GIF, MIFF, and MNG animation sequences typically start with an image background and each subsequent image varies in size and offset.  returns a new sequence where each image in the sequence is the same size as the first and composited with the next image in the sequence.
*/
PHP_METHOD(imagick, coalesceimages)
{
	MagickWand *tmp_wand;
	php_imagick_object *intern, *intern_return;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = MagickCoalesceImages(intern->magick_wand);

	if (tmp_wand == NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Coalesce image failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);
	return;

}
/* }}} */

/* {{{ proto bool Imagick::colorFloodfillImage(ImagickPixel fill, double fuzz, ImagickPixel bordercolor, int x, int y)
	Changes the color value of any pixel that matches target and is an immediate neighbor.
*/
PHP_METHOD(imagick, colorfloodfillimage)
{
	php_imagick_object *intern;
	zval *fill_param, *border_param;
	long x, y;
	double fuzz;
	MagickBooleanType status;
	PixelWand *fill_wand, *border_wand;
	zend_bool fill_allocated = 0, border_allocated = 0;

	IMAGICK_METHOD_DEPRECATED("Imagick", "colorFloodFillImage");

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zdzll", &fill_param, &fuzz, &border_param, &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	fill_wand = php_imagick_zval_to_pixelwand (fill_param, IMAGICK_CLASS, &fill_allocated TSRMLS_CC);
	if (!fill_wand)
		return;

	border_wand = php_imagick_zval_to_pixelwand (border_param, IMAGICK_CLASS, &border_allocated TSRMLS_CC);
	if (!border_wand) {
		if (fill_allocated)
			fill_wand = DestroyPixelWand (fill_wand);
		return;
	}

	status = MagickColorFloodfillImage(intern->magick_wand, fill_wand, fuzz, border_wand, x, y);

	if (fill_allocated)
		fill_wand = DestroyPixelWand (fill_wand);

	if (border_allocated)
		border_wand = DestroyPixelWand (border_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to color floodfill image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::combineImages()
	Combines one or more images into a single image. The grayscale value of the pixels of each image in the sequence is assigned in order to the specified channels of the combined image. The typical ordering would be image 1 => Red, 2 => Green, 3 => Blue, etc.
*/
PHP_METHOD(imagick, combineimages)
{
	MagickWand *tmp_wand;
	php_imagick_object *intern, *intern_return;
	long channel_type;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &channel_type) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	tmp_wand = MagickCombineImages(intern->magick_wand, channel_type);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Combine images failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto Imagick Imagick::setImage(Imagick replace )
	Replaces the current sequence
*/
PHP_METHOD(imagick, setimage)
{
	zval *objvar;
	MagickBooleanType status;
	php_imagick_object *intern, *replace;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &objvar, php_imagick_sc_entry) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	replace = (php_imagick_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(replace->magick_wand);

	status = MagickSetImage(intern->magick_wand, replace->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set the image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;

}
/* }}} */

/* {{{ proto Imagick Imagick::getImage()
	Returns a new Imagick object with the current image sequence.
*/
PHP_METHOD(imagick, getimage)
{
	MagickWand *tmp_wand;
	php_imagick_object *intern, *intern_return;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = MagickGetImage(intern->magick_wand);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Get image failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto bool Imagick::addImage(Imagick source )
	Adds new image to Imagick object from the current position of the source object.
*/
PHP_METHOD(imagick, addimage)
{
	php_imagick_object *intern, *intern_add;
	zval *add_obj;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &add_obj, php_imagick_sc_entry) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	intern_add = (php_imagick_object *)zend_object_store_get_object(add_obj TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern_add->magick_wand);
	
	status = MagickAddImage(intern->magick_wand, intern_add->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to add image" TSRMLS_CC);
		return;
	}

	MagickSetLastIterator(intern->magick_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::newImage(int cols, int rows, ImagickPixel background[, string format] )
	Creates a new image and associates ImagickPixel value as background color
*/
PHP_METHOD(imagick, newimage)
{
	php_imagick_object *intern;
	zval *param;
	MagickBooleanType status;
	long columns, rows;
	char *format = NULL;
	int format_len = 0;
	PixelWand *color_wand;
	zend_bool allocated;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llz|s", &columns, &rows, &param, &format, &format_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickNewImage(intern->magick_wand, columns, rows, color_wand);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to create new image" TSRMLS_CC);
		return;
	}

	/* If the optional fourth parameter was given
		set the image format here */
	if (format != NULL && format_len > 0) {

		status = MagickSetImageFormat(intern->magick_wand, format);

		/* No magick is going to happen */
		if (status == MagickFalse) {
			php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set the image format" TSRMLS_CC);
			return;
		}
	}

	MagickSetLastIterator(intern->magick_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::newPseudoImage(int cols, int rows, string pseudoString )
	Creates a new image using pseudo format
*/
PHP_METHOD(imagick, newpseudoimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long columns, rows;
	char *pseudo_string;
	int pseudo_string_len;
	struct php_imagick_file_t file = {0};
	php_imagick_rw_result_t rc;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lls", &columns, &rows, &pseudo_string, &pseudo_string_len) == FAILURE) {
		return;
	}

	/* Allow only pseudo formats in this method */
	if (strchr (pseudo_string, ':') == NULL) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Invalid pseudo format string" TSRMLS_CC);
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	/* Pseudo image needs a size set manually */
	status = MagickSetSize(intern->magick_wand, columns, rows);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to create new pseudo image" TSRMLS_CC);
		return;
	}

	if (!php_imagick_file_init(&file, pseudo_string, pseudo_string_len TSRMLS_CC)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The filename is too long" TSRMLS_CC);
		return;
	}
	rc = php_imagick_read_file(intern, &file, ImagickReadImage TSRMLS_CC);
	php_imagick_file_deinit(&file);

	if (rc != IMAGICK_RW_OK) {
		php_imagick_rw_fail_to_exception (intern->magick_wand, rc, pseudo_string TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::getImageTotalInkDensity()
	Gets the image total ink density.
*/
PHP_METHOD(imagick, getimagetotalinkdensity)
{
	php_imagick_object *intern;
	double ink_density;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	ink_density = MagickGetImageTotalInkDensity(intern->magick_wand);
	RETVAL_DOUBLE(ink_density);
}
/* }}} */

/* {{{ proto bool Imagick::implodeImage(float radius )
	Creates a new image that is a copy of an existing one with the image pixels "implode" by the specified percentage. It allocates the memory necessary for the new Image structure and returns a pointer to the new image.
*/
PHP_METHOD(imagick, implodeimage)
{
	php_imagick_object *intern;
	double radius;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &radius) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickImplodeImage(intern->magick_wand, radius);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to implode image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::levelImage(float blackPoint, float gamma, float whitePoint[, int channel] )
	Adjusts the levels of an image by scaling the colors falling between specified white and black points to the full available quantum range. The parameters provided represent the black, mid, and white points. The black point specifies the darkest color in the image. Colors darker than the black point are set to zero. Mid point specifies a gamma correction to apply to the image.  White point specifies the lightest color in the image. Colors brighter than the white point are set to the maximum quantum value.
*/
PHP_METHOD(imagick, levelimage)
{
	php_imagick_object *intern;
	double black_point, gamma, white_point;
	MagickBooleanType status;
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd|l", &black_point, &gamma, &white_point, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickLevelImageChannel(intern->magick_wand, channel, black_point, gamma, white_point);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to level image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::magnifyImage()
	Is a convenience method that scales an image proportionally to twice its original size.
*/
PHP_METHOD(imagick, magnifyimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickMagnifyImage(intern->magick_wand);
		
	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to magnify image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::cycleColormapImage(Imagick map, bool dither)
	Replaces the colors of an image with the closest color from a reference image.
*/
PHP_METHOD(imagick, mapimage)
{
	php_imagick_object *intern, *intern_map;
	zval *map_obj;
	zend_bool dither;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Ob", &map_obj, php_imagick_sc_entry, &dither) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	intern_map = (php_imagick_object *)zend_object_store_get_object(map_obj TSRMLS_CC);
	status = MagickMapImage(intern->magick_wand, intern_map->magick_wand, dither);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to map image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::matteFloodfillImage(float alpha,float fuzz,ImagickPixel bordercolor, int x, int y)
	Changes the transparency value of any pixel that matches target and is an immediate neighbor
*/
PHP_METHOD(imagick, mattefloodfillimage)
{
	php_imagick_object *intern;
	zval *param;
	long x, y;
	double alpha, fuzz;
	MagickBooleanType status;
	PixelWand *color_wand;
	zend_bool allocated;

	IMAGICK_METHOD_DEPRECATED("Imagick", "matteFloodfillImage");

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddzll", &alpha, &fuzz, &param, &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickMatteFloodfillImage(intern->magick_wand, alpha, fuzz, color_wand, x, y);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to matte floodfill image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::medianFilterImage(float radius)
	Applies a digital filter that improves the quality of a noisy image.  Each pixel is replaced by the median in a set of neighboring pixels as defined by radius.
*/
PHP_METHOD(imagick, medianfilterimage)
{
	php_imagick_object *intern;
	double radius;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &radius) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickMedianFilterImage(intern->magick_wand, radius);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to median filter image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::negateImage(bool gray[, int channel] )
	Negates the colors in the reference image.  The Grayscale option means that only grayscale values within the image are negated.
*/
PHP_METHOD(imagick, negateimage)
{
	php_imagick_object *intern;
	zend_bool gray;
	MagickBooleanType status;
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b|l", &gray, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickNegateImageChannel(intern->magick_wand, channel, gray);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to negate image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::paintOpaqueImage(ImagickPixel target, ImagickPixel fill, float fuzz[, int channel])
	Changes any pixel that matches color with the color defined by fill. Channel argument is supported in ImageMagick 6.2.8+.
*/
PHP_METHOD(imagick, paintopaqueimage)
{
	php_imagick_object *intern;
	zval *target_param, *fill_param;
	double fuzz;
	MagickBooleanType status;
	long channel = DefaultChannels;
	PixelWand *target_wand, *fill_wand;
	zend_bool target_allocated = 0, fill_allocated = 0;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzd|l", &target_param, &fill_param, &fuzz, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	target_wand = php_imagick_zval_to_pixelwand (target_param, IMAGICK_CLASS, &target_allocated TSRMLS_CC);
	if (!target_wand)
		return;

	fill_wand = php_imagick_zval_to_pixelwand (fill_param, IMAGICK_CLASS, &fill_allocated TSRMLS_CC);
	if (!fill_wand) {
		if (target_allocated)
			target_wand = DestroyPixelWand (target_wand);
		return;
	}

#if MagickLibVersion > 0x628
	status = MagickPaintOpaqueImageChannel(intern->magick_wand, channel, target_wand, fill_wand, fuzz);
#else
	status = MagickPaintOpaqueImage(intern->magick_wand, target_wand, fill_wand, fuzz);
#endif

	if (fill_allocated)
		fill_wand = DestroyPixelWand (fill_wand);

	if (target_allocated)
		target_wand = DestroyPixelWand (target_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable paint opaque image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */
#if MagickLibVersion > 0x628
/* {{{ proto bool Imagick::optimizeImageLayers()
	Compares each image the GIF disposed forms of the previous image in the sequence.  From this it attempts to select the smallest cropped image to replace each frame, while preserving the results of the animation.
*/
PHP_METHOD(imagick, optimizeimagelayers)
{
	MagickWand *tmp_wand;
	php_imagick_object *intern, *intern_return;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = MagickOptimizeImageLayers(intern->magick_wand);

	if (tmp_wand == NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Optimize image layers failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;
}
/* }}} */
#endif

/* {{{ proto bool Imagick::paintTransparentImage(ImagickPixel target, float alpha, float fuzz)
	Changes any pixel that matches color with the color defined by fill.
*/
PHP_METHOD(imagick, painttransparentimage)
{
	php_imagick_object *intern;
	zval *param;
	double alpha, fuzz;
	MagickBooleanType status;
	PixelWand *color_wand;
	zend_bool allocated;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zdd", &param, &alpha, &fuzz) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickPaintTransparentImage(intern->magick_wand, color_wand, alpha, fuzz);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to paint transparent image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::paintTransparentImage(int preview )
	Tiles 9 thumbnails of the specified image with an image processing operation applied at varying strengths.
	This is helpful to quickly pin-point an appropriate parameter for an image processing operation.
*/
PHP_METHOD(imagick, previewimages)
{
	php_imagick_object *intern, *intern_return;
	long preview;
	MagickWand *tmp_wand;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &preview) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = MagickPreviewImages(intern->magick_wand, preview);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Preview images failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);
	return;
}
/* }}} */

/* {{{ proto bool Imagick::profileImage(string name,string profile)
	Adds or removes a ICC, IPTC, or generic profile from an image.  If the profile is NULL, it is removed from the image otherwise added.  Use a name of '*' and a profile of NULL to remove all profiles from the image.
*/
PHP_METHOD(imagick, profileimage)
{
	php_imagick_object *intern;
	char *name, *profile;
	int name_len, profile_len;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &name, &name_len, &profile, &profile_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickProfileImage(intern->magick_wand, name, profile, profile_len);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to profile image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::quantizeImage(int numberColors, int colorspace, int treedepth, bool dither,
    bool measureError)
	Analyzes the colors within a reference image
*/
PHP_METHOD(imagick, quantizeimage)
{
	php_imagick_object *intern;
	long number_colors, colorspace, tree_depth;
	zend_bool dither, measure_error;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lllbb", &number_colors, &colorspace, &tree_depth, &dither, &measure_error) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickQuantizeImage(intern->magick_wand, number_colors, colorspace, tree_depth, dither, measure_error);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to quantize image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::quantizeImages(int number_colors, int colorspace, int treedepth, bool dither,
    bool measure_error)
	Analyzes the colors within a sequence of images
*/
PHP_METHOD(imagick, quantizeimages)
{
	php_imagick_object *intern;
	long number_colors, colorspace, tree_depth;
	zend_bool dither, measure_error;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lllbb", &number_colors, &colorspace, &tree_depth, &dither, &measure_error) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickQuantizeImages(intern->magick_wand, number_colors, colorspace, tree_depth, dither, measure_error);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to quantize images" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::reduceNoiseImage(float radius)
	Smooths the contours of an image
*/
PHP_METHOD(imagick, reducenoiseimage)
{
	php_imagick_object *intern;
	double radius;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &radius) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickReduceNoiseImage(intern->magick_wand, radius);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to reduce image noise" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string Imagick::removeImageProfile(string name)
	Removes the named image profile and returns it
*/
PHP_METHOD(imagick, removeimageprofile)
{
	php_imagick_object *intern;
	char *name;
	unsigned char *profile;
	int name_len;
#if MagickLibVersion < 0x628
	long profile_len;
#else
	size_t profile_len;
#endif

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	profile = MagickRemoveImageProfile(intern->magick_wand, name, &profile_len);

	if (!profile) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The image profile does not exist" TSRMLS_CC);
		return;
	}

	ZVAL_STRING(return_value, (char *)profile, 1);
	IMAGICK_FREE_MEMORY(unsigned char *, profile);
	return;
}
/* }}} */

/* {{{ proto bool Imagick::separateImageChannel(int channel)
	Separates a channel from the image
*/
PHP_METHOD(imagick, separateimagechannel)
{
	php_imagick_object *intern;
	long channel;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSeparateImageChannel(intern->magick_wand, channel);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to separate image channel" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::sepiaToneImage(float threshold)
	Applies a special effect to the image
*/
PHP_METHOD(imagick, sepiatoneimage)
{
	php_imagick_object *intern;
	double threshold;
	double c_opacity;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &threshold) == FAILURE) {
		return;
	}

	c_opacity = (threshold * QuantumRange ) / 100;

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSepiaToneImage(intern->magick_wand, c_opacity);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to sepia tone image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageBias(float bias)
	Sets the image bias
*/
PHP_METHOD(imagick, setimagebias)
{
	php_imagick_object *intern;
	double bias;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &bias) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageBias(intern->magick_wand, bias);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image bias" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageBluePrimary(float x,float y)
	Sets the image chromaticity blue primary point
*/
PHP_METHOD(imagick, setimageblueprimary)
{
	php_imagick_object *intern;
	double x, y;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageBluePrimary(intern->magick_wand, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image blue primary" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageBorderColor(ImagickPixel border)
	Sets the image border color
*/
PHP_METHOD(imagick, setimagebordercolor)
{
	zval *param;
	php_imagick_object *intern;
	MagickBooleanType status;
	PixelWand *color_wand;
	zend_bool allocated;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickSetImageBorderColor(intern->magick_wand, color_wand);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image border color" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageChannelDepth(int channel, int depth)
	Sets the depth of a particular image channel
*/
PHP_METHOD(imagick, setimagechanneldepth)
{
	php_imagick_object *intern;
	long channel_type, depth;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &channel_type, &depth) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageChannelDepth(intern->magick_wand, channel_type, depth);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image channel depth" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageColormapColor(int index, ImagickPixel color)
	Sets the color of the specified colormap index
*/
PHP_METHOD(imagick, setimagecolormapcolor)
{
	php_imagick_object *intern;
	zval *param;
	long index;
	MagickBooleanType status;
	PixelWand *color_wand;
	zend_bool allocated;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &index, &param) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickSetImageColormapColor(intern->magick_wand, index, color_wand);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image color map color" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;

}
/* }}} */

/* {{{ proto bool Imagick::setImageColorspace(int colorspace)
	Sets the image colorspace
*/
PHP_METHOD(imagick, setimagecolorspace)
{
	php_imagick_object *intern;
	long colorspace;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &colorspace) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageColorspace(intern->magick_wand, colorspace);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image colorspace" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageDispose(int dispose)
	Sets the image disposal method
*/
PHP_METHOD(imagick, setimagedispose)
{
	php_imagick_object *intern;
	long dispose;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &dispose) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageDispose(intern->magick_wand, dispose);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image dispose" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageExtent(int columns, int rows)
	Sets the image size
*/
PHP_METHOD(imagick, setimageextent)
{
	php_imagick_object *intern;
	long rows, columns;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &rows, &columns) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageExtent(intern->magick_wand, rows, columns);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image extent" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageGreenPrimary(float x, float y)
	Sets the image chromaticity green primary point
*/
PHP_METHOD(imagick, setimagegreenprimary)
{
	php_imagick_object *intern;
	double x, y;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageGreenPrimary(intern->magick_wand, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image green primary" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageInterlaceScheme(int interlace_scheme)
	Sets the image interlace scheme
*/
PHP_METHOD(imagick, setimageinterlacescheme)
{
	php_imagick_object *intern;
	long interlace;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &interlace) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageInterlaceScheme(intern->magick_wand, interlace);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image interlace scheme" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageProfile(string name, string profile)
	Adds a named profile to the Imagick object
*/
PHP_METHOD(imagick, setimageprofile)
{
	php_imagick_object *intern;
	char *name, *profile;
	int profile_len, name_len;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &name, &name_len, &profile, &profile_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageProfile(intern->magick_wand, name, profile, profile_len);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image profile" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageRedPrimary(float x,float y)
	Sets the image chromaticity red primary point.
*/
PHP_METHOD(imagick, setimageredprimary)
{
	php_imagick_object *intern;
	double x, y;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageRedPrimary(intern->magick_wand, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image red primary" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageRenderingIntent(int rendering_intent)
	Sets the image rendering intent.
*/
PHP_METHOD(imagick, setimagerenderingintent)
{
	php_imagick_object *intern;
	long rendering_intent;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &rendering_intent) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageRenderingIntent(intern->magick_wand, rendering_intent);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image rendering intent" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageVirtualPixelMethod(int method)
	Sets the image virtual pixel method.
*/
PHP_METHOD(imagick, setimagevirtualpixelmethod)
{
	php_imagick_object *intern;
	long virtual_pixel;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &virtual_pixel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	MagickSetImageVirtualPixelMethod(intern->magick_wand, virtual_pixel);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageWhitePoint(float x,float y)
	Sets the image chromaticity white point.
*/
PHP_METHOD(imagick, setimagewhitepoint)
{
	php_imagick_object *intern;
	double x, y;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageWhitePoint(intern->magick_wand, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image white point" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::sigmoidalContrastImage(bool sharpen, float alpha, float beta[, int channel])
	Adjusts the contrast of an image with a non-linear sigmoidal contrast algorithm.  Increase the contrast of the image using a sigmoidal transfer function without saturating highlights or shadows.  Contrast indicates how much to increase the contrast (0 is none; 3 is typical; 20 is pushing it); mid-point indicates where midtones fall in the resultant image (0 is white; 50 is middle-gray; 100 is black).  Set sharpen to true to increase the image contrast otherwise the contrast is reduced.
*/
PHP_METHOD(imagick, sigmoidalcontrastimage)
{
	php_imagick_object *intern;
	zend_bool sharpen;
	double alpha, beta;
	MagickBooleanType status;
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "bdd|l", &sharpen, &alpha, &beta, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSigmoidalContrastImageChannel(intern->magick_wand, channel, sharpen, alpha, beta);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to sigmoidal contrast image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::stereoImage(Imagick offset_wand)
	Composites two images and produces a single image that is the composite of a left and right image of a stereo pair
*/
PHP_METHOD(imagick, stereoimage)
{
	MagickWand *tmp_wand;
	zval *magick_object;
	php_imagick_object *intern, *intern_second, *intern_return;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &magick_object, php_imagick_sc_entry) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	intern_second = (php_imagick_object *)zend_object_store_get_object(magick_object TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern_second->magick_wand);

	tmp_wand = MagickStereoImage(intern->magick_wand, intern_second->magick_wand);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Stereo image failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto bool Imagick::textureImage(Imagick texture_wand)
	Repeatedly tiles the texture image across and down the image canvas.
*/
PHP_METHOD(imagick, textureimage)
{
	MagickWand *tmp_wand;
	zval *magick_object;
	php_imagick_object *intern, *intern_second, *intern_return;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &magick_object, php_imagick_sc_entry) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	intern_second = (php_imagick_object *)zend_object_store_get_object(magick_object TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern_second->magick_wand);

	tmp_wand = MagickTextureImage(intern->magick_wand, intern_second->magick_wand);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Texture image failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto bool Imagick::tintImage(ImagickPixel tint, ImagickPixel opacity)
	Applies a color vector to each pixel in the image
*/
PHP_METHOD(imagick, tintimage)
{
	php_imagick_object *intern;
	zval *tint_param, *opacity_param;
	MagickBooleanType status;
	PixelWand *tint_wand, *opacity_wand;
	zend_bool tint_allocated, opacity_allocated;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &tint_param, &opacity_param) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tint_wand = php_imagick_zval_to_pixelwand (tint_param, IMAGICK_CLASS, &tint_allocated TSRMLS_CC);
	if (!tint_wand)
		return;

	opacity_wand = php_imagick_zval_to_opacity (opacity_param, IMAGICK_CLASS, &opacity_allocated TSRMLS_CC);
	if (!opacity_wand) {
		if (tint_allocated)
			tint_wand = DestroyPixelWand (tint_wand);
		return;
	}

	status = MagickTintImage(intern->magick_wand, tint_wand, opacity_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable tint image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::unsharpMaskImage(float radius, float sigma, float amount, float threshold[, int channel])
	Sharpens an image.  We convolve the image with a Gaussian operator of the given radius and standard deviation (sigma). For reasonable results, radius should be larger than sigma.  Use a radius of 0 and Imagick::UnsharpMaskImage() selects a suitable radius for you.
*/
PHP_METHOD(imagick, unsharpmaskimage)
{
	php_imagick_object *intern;
	double radius, sigma, amount, threshold;
	MagickBooleanType status;
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd|l", &radius, &sigma, &amount, &threshold, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickUnsharpMaskImageChannel(intern->magick_wand, channel, radius, sigma, amount, threshold);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to unsharp mask image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::convolveImage(array kernel[, int channel])
	Applies a custom convolution kernel to the image.
*/
PHP_METHOD(imagick, convolveimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	zval *kernel_array;
	double *kernel;
	long channel = DefaultChannels;
	unsigned long order = 0;
	long num_elements = 0;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a|l",  &kernel_array, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	kernel = php_imagick_zval_to_double_array(kernel_array, &num_elements TSRMLS_CC);

	if (!kernel) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Unable to read matrix array" TSRMLS_CC);
		return;
	}

	order = (unsigned long) sqrt(num_elements);

	if (pow((double)order, 2) != num_elements) {
		efree(kernel);
		php_imagick_throw_exception(IMAGICK_CLASS, "The kernel must contain a square number of elements" TSRMLS_CC);
		return;
	}

	status = MagickConvolveImageChannel(intern->magick_wand, channel, order, kernel);
	efree(kernel);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to convolve image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::cycleColormapImage(int displace)
	Displaces an image's colormap by a given number of positions.  If you cycle the colormap a number of times you can produce a psychodelic effect.
*/
PHP_METHOD(imagick, cyclecolormapimage)
{
	php_imagick_object *intern;
	long displace;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &displace) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickCycleColormapImage(intern->magick_wand, displace);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to cycle image colormap" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::deconstructImages()
	Compares each image with the next in a sequence and returns the maximum bounding region of any pixel differences it discovers.
*/
PHP_METHOD(imagick, deconstructimages)
{
	MagickWand *tmp_wand;
	php_imagick_object *intern, *intern_return;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = MagickDeconstructImages(intern->magick_wand);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Deconstruct image failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto bool Imagick::getImageRegion(int width, int height, int x, int y)
	Extracts a region of the image and returns it as a a new wand.
*/
PHP_METHOD(imagick, getimageregion)
{
	MagickWand *tmp_wand;
	php_imagick_object *intern, *intern_return;
	long width, height, x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &width, &height, &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = MagickGetImageRegion(intern->magick_wand, width, height, x, y);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Get image region failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;

}
/* }}} */

/* {{{ proto bool Imagick::despeckleImage()
	Reduces the speckle noise in an image while perserving the edges of the original image.
*/
PHP_METHOD(imagick, despeckleimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickDespeckleImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to despeckle image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::edgeImage(float radius)
	Enhance edges within the image with a convolution filter of the given radius.  Use a radius of 0 and Edge() selects a suitable radius for you.
*/
PHP_METHOD(imagick, edgeimage)
{
	php_imagick_object *intern;
	double radius;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &radius) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickEdgeImage(intern->magick_wand, radius);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to edge image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;

}
/* }}} */

/* {{{ proto bool Imagick::embossImage(float radius, float sigma)
	Returns a grayscale image with a three-dimensional effect.  We convolve the image with a Gaussian operator of the given radius and standard deviation (sigma).  For reasonable results, radius should be larger than sigma.  Use a radius of 0 and it will choose a suitable radius for you.
*/
PHP_METHOD(imagick, embossimage)
{
	php_imagick_object *intern;
	double radius, sigma;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &radius, &sigma) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickEmbossImage(intern->magick_wand, radius, sigma);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to emboss image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::enhanceImage()
	Applies a digital filter that improves the quality of a noisy image.
*/
PHP_METHOD(imagick, enhanceimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickEnhanceImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to enchance image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::equalizeImage()
	Equalizes the image histogram.
*/
PHP_METHOD(imagick, equalizeimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickEqualizeImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to equalize image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::evaluateImage(int op, float constant[, int channel])
	Applys an arithmetic, relational, or logical expression to an image.  Use these operators to lighten or darken an image, to increase or decrease contrast in an image, or to produce the "negative" of an image.
*/
PHP_METHOD(imagick, evaluateimage)
{
	php_imagick_object *intern;
	long evaluate_operator;
	double constant;
	MagickBooleanType status;
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ld|l", &evaluate_operator, &constant, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickEvaluateImageChannel(intern->magick_wand, channel, evaluate_operator, constant);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to evaluate image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto array Imagick::getImageGeometry()
	Returns the width and height as an associative array.
*/
PHP_METHOD(imagick, getimagegeometry)
{
	long width,height;
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	width = MagickGetImageWidth(intern->magick_wand);
	height = MagickGetImageHeight(intern->magick_wand);

	array_init(return_value);
	add_assoc_long(return_value, "width", width);
	add_assoc_long(return_value, "height", height);

	return;
}
/* }}} */

/* {{{ proto ImagickPixel Imagick::getImageAttribute(string key )
	Returns a named attribute
*/
PHP_METHOD(imagick, getimageattribute)
{
	php_imagick_object *intern;
	char *key, *attribute;
	int key_len;

	IMAGICK_METHOD_DEPRECATED("Imagick", "getImageAttribute");

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	attribute = MagickGetImageAttribute(intern->magick_wand, key);

	if (!attribute) {
		RETURN_FALSE;
	}

	ZVAL_STRING(return_value, attribute, 1);
	IMAGICK_FREE_MEMORY(char *, attribute);

	return;
}
/* }}} */

/* {{{ proto ImagickPixel Imagick::getImageBackgroundColor()
	Returns the image background color.
*/
PHP_METHOD(imagick, getimagebackgroundcolor)
{
	php_imagick_object *intern;
	php_imagickpixel_object *internp;
	MagickBooleanType status;
	PixelWand *tmp_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = NewPixelWand();
	status = MagickGetImageBackgroundColor(intern->magick_wand, tmp_wand);

	if (tmp_wand == (PixelWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image background color" TSRMLS_CC);
		return;
	}

	if (status == MagickFalse) {
		tmp_wand = DestroyPixelWand(tmp_wand);
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image background color" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagickpixel_sc_entry);
	internp = (php_imagickpixel_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICKPIXEL_REPLACE_PIXELWAND(internp, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto ImagickPixel Imagick::getImageBluePrimary(float x, float y)
	Returns the chromaticy blue primary point for the image.
*/
PHP_METHOD(imagick, getimageblueprimary)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double x, y;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGetImageBluePrimary(intern->magick_wand, &x, &y);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image blue primary" TSRMLS_CC);
		return;
	}

	array_init(return_value);
	add_assoc_double(return_value, "x", x);
	add_assoc_double(return_value, "y", y);

	return;
}
/* }}} */

/* {{{ proto ImagickPixel Imagick::getImageBorderColor()
	Returns the image border color.
*/
PHP_METHOD(imagick, getimagebordercolor)
{
	php_imagick_object *intern;
	php_imagickpixel_object *internp;
	MagickBooleanType status;
	PixelWand *tmp_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = NewPixelWand();
	status = MagickGetImageBorderColor(intern->magick_wand, tmp_wand);

	if (tmp_wand == (PixelWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image border color" TSRMLS_CC);
		return;
	}

	if (status == MagickFalse) {
		tmp_wand = DestroyPixelWand(tmp_wand);
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image border color" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagickpixel_sc_entry);
	internp = (php_imagickpixel_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICKPIXEL_REPLACE_PIXELWAND(internp, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto int Imagick::getImageChannelDepth()
	Gets the depth for a particular image channel.
*/
PHP_METHOD(imagick, getimagechanneldepth)
{
	php_imagick_object *intern;
	long channel_type, channel_depth;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &channel_type) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	channel_depth = MagickGetImageChannelDepth(intern->magick_wand, channel_type);
	RETVAL_LONG(channel_depth);
}
/* }}} */

/* {{{ proto float Imagick::getImageChannelDistortion(Imagick reference, int channel, int metric)
	Compares one or more image channels of an image to a reconstructed image and returns the specified distortion metric.
*/
PHP_METHOD(imagick, getimagechanneldistortion)
{
	zval *objvar;
	php_imagick_object *intern, *intern_second;
	long channel_type, metric_type;
	double distortion;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Oll", &objvar, php_imagick_sc_entry, &channel_type, &metric_type) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	intern_second = (php_imagick_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern_second->magick_wand);

	status = MagickGetImageChannelDistortion(intern->magick_wand, intern_second->magick_wand, channel_type, metric_type, &distortion);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image channel distortion" TSRMLS_CC);
		return;
	}

	RETVAL_DOUBLE(distortion);

}
/* }}} */

/* {{{ proto array Imagick::getImageChannelExtrema(int channel)
	Gets the extrema for one or more image channels.  Return value is an associative array with the keys "minima" and "maxima".
*/
PHP_METHOD(imagick, getimagechannelextrema)
{
	php_imagick_object *intern;
	long channel_type;
	unsigned long minima, maxima;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &channel_type) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGetImageChannelExtrema(intern->magick_wand, channel_type, &minima, &maxima);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image channel extrema" TSRMLS_CC);
		return;
	}

	array_init(return_value);
	add_assoc_long(return_value, "minima", minima);
	add_assoc_long(return_value, "maxima", maxima);

	return;
}
/* }}} */

/* {{{ proto array Imagick::getImageChannelMean(int channel)
	Gets the mean and standard deviation of one or more image channels.  Return value is an associative array with the keys "mean" and "standardDeviation".
*/
PHP_METHOD(imagick, getimagechannelmean)
{
	php_imagick_object *intern;
	long channel_type;
	double mean, standard_deviation;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &channel_type) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGetImageChannelMean(intern->magick_wand, channel_type, &mean, &standard_deviation);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image channel mean" TSRMLS_CC);
		return;
	}

	array_init(return_value);
	add_assoc_double(return_value, "mean", mean);
	add_assoc_double(return_value, "standardDeviation", standard_deviation);

	return;
}
/* }}} */

/* {{{ proto array Imagick::getImageChannelStatistics()
	Returns statistics for each channel in the image.  The statistics incude the channel depth, its minima and maxima, the mean, and the standard deviation.  You can access the red channel mean, for example, like this:
*/
PHP_METHOD(imagick, getimagechannelstatistics)
{
	zval *tmp;

	const long channels[] = { UndefinedChannel, RedChannel, CyanChannel,
							  GreenChannel, MagentaChannel, BlueChannel,
							  YellowChannel, OpacityChannel, BlackChannel,
							  MatteChannel };
	php_imagick_object *intern;
	ChannelStatistics *statistics;
	int elements = 10, i;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	statistics = MagickGetImageChannelStatistics(intern->magick_wand);
	array_init(return_value);

	for (i = 0; i < elements ; i++) {

		MAKE_STD_ZVAL(tmp);
		array_init(tmp);

		add_assoc_double(tmp, "mean", statistics[channels[i]].mean);
		add_assoc_double(tmp, "minima", statistics[channels[i]].minima);
		add_assoc_double(tmp, "maxima", statistics[channels[i]].maxima);
		add_assoc_double(tmp, "standardDeviation", statistics[channels[i]].standard_deviation);
#if MagickLibVersion < 0x635
		add_assoc_long(tmp, "scale", statistics[channels[i]].scale);
#endif
		add_assoc_long(tmp, "depth", statistics[channels[i]].depth);
		add_index_zval(return_value, channels[i], tmp);
	}
	return;
}
/* }}} */

/* {{{ proto ImagickPixel Imagick::getImageColormapColor(int index, ImagickPixel color)
	Returns the color of the specified colormap index.
*/
PHP_METHOD(imagick, getimagecolormapcolor)
{
	php_imagick_object *intern;
	php_imagickpixel_object *internp;
	MagickBooleanType status;
	PixelWand *tmp_wand;
	long index;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = NewPixelWand();
	status = MagickGetImageColormapColor(intern->magick_wand, index , tmp_wand);

	if (tmp_wand == (PixelWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image colormap color" TSRMLS_CC);
		return;
	}

	if (status == MagickFalse) {
		tmp_wand = DestroyPixelWand(tmp_wand);
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image colormap color" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagickpixel_sc_entry);
	internp = (php_imagickpixel_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICKPIXEL_REPLACE_PIXELWAND(internp, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto int Imagick::getImageColorspace()
	Gets the image colorspace.
*/
PHP_METHOD(imagick, getimagecolorspace)
{
	php_imagick_object *intern;
	long colorSpace;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	colorSpace = MagickGetImageColorspace(intern->magick_wand);
	RETVAL_LONG(colorSpace);
}
/* }}} */

/* {{{ proto int Imagick::getImageCompose()
	Returns the composite operator associated with the image.
*/
PHP_METHOD(imagick, getimagecompose)
{
	php_imagick_object *intern;
	long composite;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	composite = MagickGetImageCompose(intern->magick_wand);
	RETVAL_LONG(composite);
}
/* }}} */

/* {{{ proto int Imagick::getImageDelay()
	Gets the image delay.
*/
PHP_METHOD(imagick, getimagedelay)
{
	php_imagick_object *intern;
	long delay;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	delay = MagickGetImageDelay(intern->magick_wand);
	RETVAL_LONG(delay);
}
/* }}} */

/* {{{ proto int Imagick::getImageDepth()
	Gets the image depth.
*/
PHP_METHOD(imagick, getimagedepth)
{
	php_imagick_object *intern;
	long depth;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	depth = MagickGetImageDepth(intern->magick_wand);
	RETVAL_LONG(depth);
}
/* }}} */

/* {{{ proto float Imagick::getImageDistortion(MagickWand reference, int metric)
	Compares an image to a reconstructed image and returns the specified distortion metric.
*/
PHP_METHOD(imagick, getimagedistortion)
{
	zval *objvar;
	php_imagick_object *intern, *intern_second;
	long metric_type;
	double distortion;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Ol", &objvar, php_imagick_sc_entry, &metric_type) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	intern_second = (php_imagick_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern_second->magick_wand);

	status = MagickGetImageDistortion(intern->magick_wand, intern_second->magick_wand, metric_type, &distortion);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image distortion" TSRMLS_CC);
		return;
	}

	RETVAL_DOUBLE(distortion);
}
/* }}} */

/* {{{ proto array Imagick::getImageExtrema()
	Gets the extrema for the image.  Returns an associative array with the keys "min" and "max".
*/
PHP_METHOD(imagick, getimageextrema)
{
	php_imagick_object *intern;
	unsigned long min, max;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGetImageExtrema(intern->magick_wand, &min, &max);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image extrema" TSRMLS_CC);
		return;
	}

	array_init(return_value);
	add_assoc_long(return_value, "min", min);
	add_assoc_long(return_value, "max", max);

	return;
}
/* }}} */

/* {{{ proto long Imagick::getImageDispose()
	Gets the image disposal method.
*/
PHP_METHOD(imagick, getimagedispose)
{
	php_imagick_object *intern;
	long dispose;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	dispose = MagickGetImageDispose(intern->magick_wand);
	RETVAL_LONG(dispose);
}
/* }}} */

/* {{{ proto float Imagick::getImageGamma()
	Gets the image gamma.
*/
PHP_METHOD(imagick, getimagegamma)
{
	php_imagick_object *intern;
	double gamma;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	gamma = MagickGetImageGamma(intern->magick_wand);
	RETVAL_DOUBLE(gamma);
}
/* }}} */

/* {{{ proto array Imagick::getImageGreenPrimary()
	Returns the chromaticy green primary point. Returns an array with the keys "x" and "y".
*/
PHP_METHOD(imagick, getimagegreenprimary)
{
	php_imagick_object *intern;
	double x, y;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGetImageGreenPrimary(intern->magick_wand, &x, &y);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image green primary" TSRMLS_CC);
		return;
	}

	array_init(return_value);
	add_assoc_double(return_value, "x", x);
	add_assoc_double(return_value, "y", y);

	return;
}
/* }}} */

/* {{{ proto int Imagick::getImageHeight()
	Returns the image height.
*/
PHP_METHOD(imagick, getimageheight)
{
	php_imagick_object *intern;
	long height;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	height = MagickGetImageHeight(intern->magick_wand);
	RETVAL_LONG(height);
}
/* }}} */

/* {{{ proto array Imagick::getImageHistogram()
	Returns the image histogram as an array of ImagickPixel objects.
*/
PHP_METHOD(imagick, getimagehistogram)
{
	php_imagick_object *intern;
	php_imagickpixel_object *internp;
	PixelWand **wand_array;
	unsigned long colors = 0;
	unsigned long i;
	zval *tmp_pixelwand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	wand_array = MagickGetImageHistogram(intern->magick_wand, &colors);
	array_init(return_value);

	for (i = 0; i < colors; i++) {
		if (wand_array[i]) {
			MAKE_STD_ZVAL(tmp_pixelwand);
			object_init_ex(tmp_pixelwand, php_imagickpixel_sc_entry);
			internp = (php_imagickpixel_object *)zend_object_store_get_object(tmp_pixelwand TSRMLS_CC);
			IMAGICKPIXEL_REPLACE_PIXELWAND(internp, wand_array[i]);
			add_next_index_zval(return_value, tmp_pixelwand);
		}
	}

	IMAGICK_FREE_MEMORY(PixelWand **, wand_array);
	return;
}
/* }}} */

/* {{{ proto int Imagick::getImageInterlaceScheme()
	Gets the image interlace scheme.
*/
PHP_METHOD(imagick, getimageinterlacescheme)
{
	php_imagick_object *intern;
	long interlace;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	interlace = MagickGetImageInterlaceScheme(intern->magick_wand);
	RETVAL_LONG(interlace);
}
/* }}} */

/* {{{ proto int Imagick::getImageIterations()
	Gets the image iterations.
*/
PHP_METHOD(imagick, getimageiterations)
{
	php_imagick_object *intern;
	long iterations;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	iterations = MagickGetImageIterations(intern->magick_wand);
	RETVAL_LONG(iterations);
}
/* }}} */

/* {{{ proto ImagickPixel Imagick::getImageMatteColor()
	Returns the image matte color.
*/
PHP_METHOD(imagick, getimagemattecolor)
{
	php_imagick_object *intern;
	php_imagickpixel_object *internp;
	MagickBooleanType status;
	PixelWand *tmp_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = NewPixelWand();
	status = MagickGetImageMatteColor(intern->magick_wand, tmp_wand);

	if (tmp_wand == (PixelWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image matte color" TSRMLS_CC);
		return;
	}

	if (status == MagickFalse) {
		tmp_wand = DestroyPixelWand(tmp_wand);
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable get image matter color" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagickpixel_sc_entry);
	internp = (php_imagickpixel_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICKPIXEL_REPLACE_PIXELWAND(internp, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto array Imagick::getImagePage()
	Returns the page geometry associated with the image in an array with the keys "width", "height", "x", and "y".
*/
PHP_METHOD(imagick, getimagepage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	unsigned long width, height;
	long x, y;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGetImagePage(intern->magick_wand, &width, &height, &x, &y);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image page" TSRMLS_CC);
		return;
	}

	array_init(return_value);

	add_assoc_long(return_value, "width", width);
	add_assoc_long(return_value, "height", height);
	add_assoc_long(return_value, "x", x);
	add_assoc_long(return_value, "y", y);

	return;
}
/* }}} */

/* {{{ proto ImagickPixel Imagick::getImagePixelColor(int x, int y)
	Returns the color of the specified pixel.
*/
PHP_METHOD(imagick, getimagepixelcolor)
{
	php_imagick_object *intern;
	php_imagickpixel_object *internp;
	MagickBooleanType status;
	PixelWand *tmp_wand;
	long x, y;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = NewPixelWand();

	if (!tmp_wand) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Failed to allocate new PixelWand" TSRMLS_CC);
		return;
	}

	status = MagickGetImagePixelColor(intern->magick_wand, x, y , tmp_wand);

	if (status == MagickFalse) {
		tmp_wand = DestroyPixelWand(tmp_wand);
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable get image pixel color" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagickpixel_sc_entry);
	internp = (php_imagickpixel_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICKPIXEL_REPLACE_PIXELWAND(internp, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto string Imagick::getImageProfile(string name)
	Returns the named image profile.
*/
PHP_METHOD(imagick, getimageprofile)
{
	php_imagick_object *intern;
	char *profile, *name;
	int name_len;
#if MagickLibVersion < 0x628
	long length;
#else
	size_t length;
#endif

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	profile = (char *)MagickGetImageProfile(intern->magick_wand, name, &length);

	if (profile) {
		ZVAL_STRINGL(return_value, profile, length, 1);
		IMAGICK_FREE_MEMORY(char *, profile);
		return;
	}

	php_imagick_throw_exception(IMAGICK_CLASS, "Can not get image profile" TSRMLS_CC);
	return;
}
/* }}} */

/* {{{ proto array Imagick::getImageRedPrimary()
	Returns the chromaticy red primary point as an array with the keys "x" and "y".
*/
PHP_METHOD(imagick, getimageredprimary)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double x, y;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGetImageRedPrimary(intern->magick_wand, &x, &y);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image red primary" TSRMLS_CC);
		return;
	}

	array_init(return_value);
	add_assoc_double(return_value, "x", x);
	add_assoc_double(return_value, "y", y);

	return;
}
/* }}} */

/* {{{ proto int Imagick::getImageRenderingIntent()
	Gets the image rendering intent.
*/
PHP_METHOD(imagick, getimagerenderingintent)
{
	php_imagick_object *intern;
	long renderingIntent;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	renderingIntent = MagickGetImageRenderingIntent(intern->magick_wand);
	RETVAL_LONG(renderingIntent);
}
/* }}} */

/* {{{ proto array Imagick::getImageResolution()
	Gets the image X and Y resolution.
*/
PHP_METHOD(imagick, getimageresolution)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double x, y;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGetImageResolution(intern->magick_wand, &x, &y);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image resolution" TSRMLS_CC);
		return;
	}

	array_init(return_value);
	add_assoc_double(return_value, "x", x);
	add_assoc_double(return_value, "y", y);

	return;
}
/* }}} */

/* {{{ proto int Imagick::getImageScene()
	Gets the image scene.
*/
PHP_METHOD(imagick, getimagescene)
{
	php_imagick_object *intern;
	unsigned long scene;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	scene = MagickGetImageScene(intern->magick_wand);
	RETVAL_LONG(scene);
}
/* }}} */

/* {{{ proto string Imagick::getImageSignature()
	Generates an SHA-256 message digest for the image pixel stream.
*/
PHP_METHOD(imagick, getimagesignature)
{
	php_imagick_object *intern;
	char *signature;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	signature = MagickGetImageSignature(intern->magick_wand);
	ZVAL_STRING(return_value, signature, 1);
	IMAGICK_FREE_MEMORY(char *, signature);
	return;
}
/* }}} */

/* {{{ proto int Imagick::getImageTicksPerSecond()
	Gets the image ticks-per-second.
*/
PHP_METHOD(imagick, getimagetickspersecond)
{
	php_imagick_object *intern;
	unsigned long ticks;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	ticks = MagickGetImageTicksPerSecond(intern->magick_wand);
	RETVAL_LONG(ticks);
}
/* }}} */

/* {{{ proto int Imagick::getImageType()
	Gets the potential image type:
*/
PHP_METHOD(imagick, getimagetype)
{
	php_imagick_object *intern;
	long imageType;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	imageType = MagickGetImageType(intern->magick_wand);
	RETVAL_LONG(imageType);
}
/* }}} */

/* {{{ proto int Imagick::getImageUnits()
	Gets the image units of resolution.
*/
PHP_METHOD(imagick, getimageunits)
{
	php_imagick_object *intern;
	long resolutionType;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	resolutionType = MagickGetImageUnits(intern->magick_wand);
	RETVAL_LONG(resolutionType);
}
/* }}} */

/* {{{ proto int Imagick::getImageVirtualPixelMethod()
	Returns the virtual pixel method for the sepcified image.
*/
PHP_METHOD(imagick, getimagevirtualpixelmethod)
{
	php_imagick_object *intern;
	long pixelMethod;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	pixelMethod = MagickGetImageVirtualPixelMethod(intern->magick_wand);
	RETVAL_LONG(pixelMethod);
}
/* }}} */

/* {{{ proto array Imagick::getImageWhitePoint()
	Returns the chromaticy white point as an associative array with the keys "x" and "y".
*/
PHP_METHOD(imagick, getimagewhitepoint)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double x, y;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGetImageWhitePoint(intern->magick_wand, &x, &y);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get image white point" TSRMLS_CC);
		return;
	}

	array_init(return_value);
	add_assoc_double(return_value, "x", x);
	add_assoc_double(return_value, "y", y);

	return;
}
/* }}} */

/* {{{ proto int Imagick::getImageWidth()
	Returns the image width.
*/
PHP_METHOD(imagick, getimagewidth)
{
	php_imagick_object *intern;
	unsigned long width;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	width = MagickGetImageWidth(intern->magick_wand);
	RETVAL_LONG(width);
}
/* }}} */

/* {{{ proto int Imagick::getNumberImages()
	Returns the number of images associated with Imagick object.
*/
PHP_METHOD(imagick, getnumberimages)
{
	php_imagick_object *intern;
	unsigned long num_images;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	num_images = MagickGetNumberImages(intern->magick_wand);
	RETVAL_LONG(num_images);
}
/* }}} */

/*
	Resizes an image so that it is 'bestfit' for the bounding box
	If the image does not fill the box completely the box is filled with
	image's background color. The background color can be set using MagickSetImageBackgroundColor
*/
#if MagickLibVersion > 0x631
static
zend_bool s_resize_bounding_box(MagickWand *magick_wand, long box_width, long box_height, zend_bool fill)
{
	long new_width, new_height;
	long extent_x, extent_y;

	/* Calculate dimensions */
	if (!php_imagick_thumbnail_dimensions(magick_wand, 1, box_width, box_height, &new_width, &new_height)) {
		return 0;
	}

	/* Resize the image to the new size */
	if (MagickThumbnailImage(magick_wand, new_width, new_height) == MagickFalse) {
		return 0;
	}

	/* If user does not want to fill we are all done here */
	if (!fill) {
		return 1;
	}

	/* In case user wants to fill use extent for it rather than creating a new canvas */
	extent_x = (box_width > new_width)   ? ((box_width - new_width) / 2)   : 0;
	extent_y = (box_height > new_height) ? ((box_height - new_height) / 2) : 0;

	if (MagickExtentImage(magick_wand, box_width, box_height, extent_x * -1, extent_y * -1) == MagickFalse) {
		return 0;
	}
	return 1;
}
#endif

/* {{{ proto bool Imagick::thumbnailImage(int columns, int rows[, bool bestfit = false, bool fill = false])
	 Changes the size of an image to the given dimensions and removes any associated profiles
*/
PHP_METHOD(imagick, thumbnailimage)
{
	long width, height, new_width, new_height;
	php_imagick_object *intern;
	zend_bool bestfit = 0, fill = 0;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|bb", &width, &height, &bestfit, &fill) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	if (bestfit && fill) {
#if MagickLibVersion > 0x631
		if (!s_resize_bounding_box(intern->magick_wand, width, height, fill)) {
			php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to resize and fill image" TSRMLS_CC);
		return;
		}
#else 
		php_imagick_throw_exception(IMAGICK_CLASS, "Fill parameter is only supported with ImageMagick 6.3.2+" TSRMLS_CC);
		return;
#endif
	} else {
		if (!php_imagick_thumbnail_dimensions(intern->magick_wand, bestfit, width, height, &new_width, &new_height)) {
			php_imagick_throw_exception(IMAGICK_CLASS, "Invalid image geometry" TSRMLS_CC);
			return;
		}
		/* No magick is going to happen */
		if (MagickThumbnailImage(intern->magick_wand, new_width, new_height) == MagickFalse) {
			php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to thumbnail image" TSRMLS_CC);
		return;
		}
	}
	RETURN_TRUE;
}
/* }}} */

static
zend_bool s_crop_thumbnail_image(MagickWand *magick_wand, long desired_width, long desired_height TSRMLS_DC)
{
	double ratio_x, ratio_y;
	long crop_x = 0, crop_y = 0, new_width, new_height;

	long orig_width  = MagickGetImageWidth(magick_wand);
	long orig_height = MagickGetImageHeight(magick_wand);

	/* Already at the size, just strip profiles */
	if ((orig_width == desired_width) && (orig_height == desired_height)) {
		if (!MagickStripImage(magick_wand)) {
			return 0;
		}
		return 1;
	}

	ratio_x = ((double) desired_width / (double) orig_width);
	ratio_y = ((double) desired_height / (double) orig_height);

	if (ratio_x > ratio_y) {
		new_width  = desired_width;
		new_height = ratio_x * (double)orig_height;
	} else {
		new_height = desired_height;
		new_width  = ratio_y * (double)orig_width;
	}

	if (MagickThumbnailImage(magick_wand, new_width, new_height) == MagickFalse) {
		return 0;
	}

	/* all done here */
	if ((new_width == desired_width) && (new_height == desired_height)) {
		return 1;
	}

	crop_x = (long) ((new_width - desired_width) / 2);
	crop_y = (long) ((new_height - desired_height) / 2);

	if (MagickCropImage(magick_wand, desired_width, desired_height, crop_x, crop_y) == MagickFalse) {
		return 0;
	}

	MagickSetImagePage(magick_wand, desired_width, desired_height, 0, 0);
	return 1;
}

// 
/* {{{ proto bool Imagick::cropthumbnailImage(int columns, int rows)
	 Creates a crop thumbnail
*/
PHP_METHOD(imagick, cropthumbnailimage)
{
	long crop_width, crop_height;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &crop_width, &crop_height) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	/* The world collapses.. */
	if (!s_crop_thumbnail_image(intern->magick_wand, crop_width, crop_height TSRMLS_CC)) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to crop-thumbnail image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::resetIterator();
	Resets the iterator.  Use it in conjunction with Imagick::nextImage() to iterate over all the images in a wand container.
*/
PHP_METHOD(imagick, resetiterator)
{
	php_imagick_object *intern;
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	/* No magick is going to happen */
	if (intern->magick_wand == (MagickWand *)NULL) {
		RETURN_FALSE;
	}
	intern->next_out_of_bound = 0;
	MagickResetIterator(intern->magick_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setFirstIterator()
	Sets the wand iterator to the first image.
*/
PHP_METHOD(imagick, setfirstiterator)
{
	php_imagick_object *intern;
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	/* No magick is going to happen */
	if (intern->magick_wand == (MagickWand *)NULL) {
		RETURN_FALSE;
	}
	intern->next_out_of_bound = 0;
	MagickSetFirstIterator(intern->magick_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setLastIterator()
	Sets the wand iterator to the last image.
*/
PHP_METHOD(imagick, setlastiterator)
{
	php_imagick_object *intern;
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	/* No magick is going to happen */
	if (intern->magick_wand == (MagickWand *)NULL) {
		RETURN_FALSE;
	}
	intern->next_out_of_bound = 0;
	MagickSetLastIterator(intern->magick_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::previousImage()
	Assocates the previous image in an image list with the Imagick object.
*/
PHP_METHOD(imagick, previousimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickPreviousImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		RETURN_FALSE;
	}
	intern->next_out_of_bound = 0;
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::nextImage()
	Associates the next image in the image list with an Imagick object.
*/
PHP_METHOD(imagick, nextimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickNextImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		intern->next_out_of_bound = 1;
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::hasPreviousImage()
	Returns true if the wand has more images when traversing the list in the reverse direction
*/
PHP_METHOD(imagick, haspreviousimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickHasPreviousImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::hasNextImage()
	Returns true if the wand has more images when traversing the list in the forward direction
*/
PHP_METHOD(imagick, hasnextimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickHasNextImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto int Imagick::getImageIndex()
	Returns the index of the current active image, within the Imagick object.
*/
PHP_METHOD(imagick, getimageindex)
{
	MagickBooleanType status;
	php_imagick_object *intern;

#if MagickLibVersion > 0x628
	IMAGICK_METHOD_DEPRECATED("Imagick", "getImageindex");
#endif

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	status = MagickGetImageIndex(intern->magick_wand);
	ZVAL_LONG(return_value, (long)status);
	return;
}
/* }}} */

/* {{{ proto bool Imagick::setImageIndex(int index)
	Returns the index of the current active image, within the Imagick object.
*/
PHP_METHOD(imagick, setimageindex)
{
	long index;
	MagickBooleanType status;
	php_imagick_object *intern;

#if MagickLibVersion > 0x628
	IMAGICK_METHOD_DEPRECATED("Imagick", "setImageIndex");
#endif

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetImageIndex(intern->magick_wand, index);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image index" TSRMLS_CC);
		return;
	}
	intern->next_out_of_bound = 0;
	RETURN_TRUE;

}
/* }}} */

/* {{{ proto bool Imagick::removeImage()
	Removes an image from the image list.
*/
PHP_METHOD(imagick, removeimage)
{
	MagickBooleanType status;
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickRemoveImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to remove image" TSRMLS_CC);
		return;
	}
	intern->next_out_of_bound = 0;
	MagickSetLastIterator(intern->magick_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string Imagick::getImageFilename()
	Returns the filename of a particular image in a sequence
*/
PHP_METHOD(imagick, getimagefilename)
{
	php_imagick_object *intern;
	char *filename;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	filename = MagickGetImageFilename(intern->magick_wand);

	if (filename == (char *)NULL) {
		return;
	}

	ZVAL_STRING(return_value, filename, 1);
	IMAGICK_FREE_MEMORY(char *, filename);
	return;
}
/* }}} */

/* {{{ proto int Imagick::getImageSize()
	returns the image length in bytes
*/
PHP_METHOD(imagick, getimagesize)
{
	php_imagick_object *intern;

	IMAGICK_METHOD_DEPRECATED_USE_INSTEAD("Imagick", "getImageSize", "Imagick", "getImageLength");

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	ZVAL_LONG(return_value, (long)MagickGetImageSize(intern->magick_wand));
	return;
}
/* }}} */

static
zend_bool s_image_has_format (MagickWand *magick_wand)
{
	char *buffer;
	buffer = MagickGetImageFormat(magick_wand);
	if (!buffer || *buffer == '\0') {
		if (buffer)
			MagickRelinquishMemory (buffer);
		return 0;
	}
	return 1;
}

/* {{{ proto string Imagick::getImageBlob()
	Returns the current image sequence as a string
*/
PHP_METHOD(imagick, getimageblob)
{
	php_imagick_object *intern;
	unsigned char *image_contents;
	size_t image_size;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	if (!s_image_has_format (intern->magick_wand)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Image has no format" TSRMLS_CC);
		return;
	}

	image_contents = MagickGetImageBlob(intern->magick_wand, &image_size);
	if (!image_contents) {
		return;
	}

	ZVAL_STRINGL(return_value, (char *)image_contents, image_size, 1);
	IMAGICK_FREE_MEMORY(unsigned char *, image_contents);
	return;
}
/* }}} */

/* {{{ proto string Imagick::getImagesBlob()
	Returns all image sequences as a string
*/
PHP_METHOD(imagick, getimagesblob)
{
	php_imagick_object *intern;
	unsigned char *image_contents;
	size_t image_size;
	int current;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

#if MagickLibVersion > 0x628
	/* Get the current iterator position */
	current = MagickGetIteratorIndex(intern->magick_wand);
#else
	/* Get the current iterator position */
	current = MagickGetImageIndex(intern->magick_wand);
#endif

	/* Reset the iterator */
	MagickResetIterator(intern->magick_wand);

	/* Loop all images to make sure they have a format */
	while (MagickNextImage(intern->magick_wand)) {
		if (!s_image_has_format (intern->magick_wand)) {
			php_imagick_throw_exception(IMAGICK_CLASS, "Image has no format" TSRMLS_CC);
			return;
		}
	}

#if MagickLibVersion > 0x628
	/* Get the current iterator position */
	status = MagickSetIteratorIndex(intern->magick_wand, current);
#else
	/* Get the current iterator position */
	status = MagickSetImageIndex(intern->magick_wand, current);
#endif

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set the iterator index" TSRMLS_CC);
		return;
	}

	image_contents = MagickGetImagesBlob(intern->magick_wand, &image_size);
	if (!image_contents) {
		return;
	}

	ZVAL_STRINGL(return_value, (char *)image_contents, image_size, 1);
	IMAGICK_FREE_MEMORY(unsigned char *, image_contents);
	return;
}
/* }}} */

/* {{{ proto string Imagick::getImageFormat()
	Returns the format of a particular image in a sequence.
*/
PHP_METHOD(imagick, getimageformat)
{
	php_imagick_object *intern;
	char *format;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	if (!s_image_has_format (intern->magick_wand)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Image has no format" TSRMLS_CC);
		return;
	}

	format = MagickGetImageFormat (intern->magick_wand);
	ZVAL_STRING(return_value, format, 1);
	IMAGICK_FREE_MEMORY (char *, format);
	return;
}
/* }}} */

/* {{{ proto string Imagick::getImageMimeType()
	Returns the image mime-type
*/
PHP_METHOD(imagick, getimagemimetype)
{
	php_imagick_object *intern;
	char *format = NULL, *mime_type = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	if (!s_image_has_format (intern->magick_wand)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Image has no format" TSRMLS_CC);
		return;
	}

	format = MagickGetImageFormat (intern->magick_wand);

	mime_type = (char *) MagickToMime(format);
	IMAGICK_FREE_MEMORY(char *, format);

	if (!mime_type) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Unable to get image mime-type" TSRMLS_CC);
		return;
	}

	ZVAL_STRING(return_value, mime_type, 1);
	IMAGICK_FREE_MEMORY(char *, mime_type);
	return;
}
/* }}} */


static
void s_add_assoc_str (zval *array, const char *key, const char *value, int copy)
{
    add_assoc_string (array, key, (char *)(value ? value : ""), copy);
}

static
void s_add_named_strings (zval *array, const char *haystack TSRMLS_DC)
{
	char *line, *last_ptr = NULL, *buffer;
	size_t num_keys;

	const char *str_keys [] = {
		"Format: ",
		"Units: ",
		"Type: ",
		"Colorspace: ",
		"Filesize: ",
		"Compression: "
	};

	const char *arr_keys [] = {
		"format",
		"units",
		"type",
		"colorSpace",
		"fileSize",
		"compression"
	};

	buffer = estrdup (haystack);

	num_keys = sizeof (str_keys) / sizeof (str_keys[0]);
	line = php_strtok_r (buffer, "\r\n", &last_ptr);

	int i, found = 0;
	while ((found < num_keys) && line) {
		// Break the line further into tokens
		char *trim = php_trim (line, strlen(line), NULL, 0, NULL, 3 TSRMLS_CC);

		for (i = 0; i < num_keys; i++) {
			if (strncmp (trim, str_keys [i], strlen (str_keys [i])) == 0) {
				// This should be our line
				add_assoc_string (array, arr_keys [i], trim + strlen (str_keys [i]), 1);
				found++;
			}
		}
		efree (trim);
		line = php_strtok_r (NULL, "\r\n", &last_ptr);
	}
	efree (buffer);
}

/* {{{ proto array Imagick::identifyImage([bool appendRawOutput] )
	Identifies an image and returns the attributes.  Attributes include the image width, height, size, and others.
	If true is passed as argument, then the raw output is appended to the array.
*/
PHP_METHOD(imagick, identifyimage)
{
	char *format, *identify, *filename;
	php_imagick_object *intern;
	zend_bool append_raw_string = 0;
	zval *array;
    double x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &append_raw_string) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	// This is to parse some string options, ugly hack but easier than using CommandOptionToMNemonic
	// or MagickOptionToMnemonic and FormatMagickSize, which have changed names and signatures between
	// versions that we support
	identify = MagickIdentifyImage (intern->magick_wand);

	// Actually generate the array ourselves
	array_init(return_value);

    // Name of the image
	filename = MagickGetImageFilename (intern->magick_wand);
	s_add_assoc_str (return_value, "imageName", filename, 1);
	IMAGICK_FREE_MEMORY(char *, filename);

	format = MagickGetImageFormat (intern->magick_wand);
	if (format) {
		char *mime_type = MagickToMime(format);
		if (mime_type) {
			s_add_assoc_str (return_value, "mimetype", mime_type, 1);
			IMAGICK_FREE_MEMORY(char *, mime_type);
		} else
			s_add_assoc_str (return_value, "mimetype", "unknown", 1);

		IMAGICK_FREE_MEMORY(char *, format);
	}
	else
		s_add_assoc_str (return_value, "mimetype", "unknown", 1);

	s_add_named_strings (return_value, identify TSRMLS_CC);

	// Geometry is an associative array
	MAKE_STD_ZVAL (array);
	array_init (array);

	add_assoc_long (array, "width", MagickGetImageWidth (intern->magick_wand));
	add_assoc_long (array, "height", MagickGetImageHeight (intern->magick_wand));
	add_assoc_zval (return_value, "geometry", array);

	if (MagickGetImageResolution(intern->magick_wand, &x, &y) == MagickTrue) {
	    MAKE_STD_ZVAL (array);
	    array_init (array);

	    add_assoc_double (array, "x", x);
	    add_assoc_double (array, "y", y);
	    add_assoc_zval (return_value, "resolution", array);
	}
	s_add_assoc_str (return_value, "signature", MagickGetImageSignature (intern->magick_wand), 1);

	if (append_raw_string == 1)
		add_assoc_string (return_value, "rawOutput", identify, 1);

	IMAGICK_FREE_MEMORY(char *, identify);
	return;
}
/* }}} */

#undef imagick_option_to_string

/* {{{ proto int Imagick::getImageColors()
	Gets the number of unique colors in the image.
*/
PHP_METHOD(imagick, getimagecolors)
{
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	ZVAL_LONG(return_value, (long)MagickGetImageColors(intern->magick_wand));
	return;
}
/* }}} */

/* {{{ proto bool Imagick::commentImage(string comment)
	Adds a comment to your image.
*/
PHP_METHOD(imagick, commentimage)
{
	php_imagick_object *intern;
	char *comment;
	int comment_len;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &comment, &comment_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickCommentImage(intern->magick_wand, comment);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to comment image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageFilename(string filename)
	Sets the filename of a particular image in a sequence.
*/
PHP_METHOD(imagick, setimagefilename)
{
	php_imagick_object *intern;
	char *filename;
	int filename_len;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filename, &filename_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageFilename(intern->magick_wand, filename);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image filename" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* PS, DEPRECATED please remove: http://www.imagemagick.org/discourse-server/viewtopic.php?f=6&t=8196 */
PHP_METHOD(imagick, setimageattribute)
{
	php_imagick_object *intern;
	char *key, *attribute;
	int key_len, attribute_len;
	MagickBooleanType status;

	/* Tell user that this method has been deprecated. */
	IMAGICK_METHOD_DEPRECATED("Imagick", "setImageAttribute");

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len, &attribute, &attribute_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageAttribute(intern->magick_wand, key, attribute);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image attribute" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}

/* {{{ proto bool Imagick::setImageBackgroundColor(ImagickPixel background)
	Sets the image background color.
*/
PHP_METHOD(imagick, setimagebackgroundcolor)
{
	zval *param;
	php_imagick_object *intern;
	MagickBooleanType status;
	PixelWand *color_wand;
	zend_bool allocated;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickSetImageBackgroundColor(intern->magick_wand, color_wand);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image background color" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageCompose(int compose)
	Sets the image composite operator, useful for specifying how to composite the image thumbnail when using the Imagick::montageImage() method.
*/
PHP_METHOD(imagick, setimagecompose)
{
	php_imagick_object *intern;
	long compose;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &compose) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageCompose(intern->magick_wand, compose);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image composite operator" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageCompression(int compression)
	Sets the image compression.
*/
PHP_METHOD(imagick, setimagecompression)
{
	php_imagick_object *intern;
	long compression;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &compression) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageCompression(intern->magick_wand, compression);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image compression" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageDelay(int delay)
	Sets the image delay.
*/
PHP_METHOD(imagick, setimagedelay)
{
	php_imagick_object *intern;
	long delay;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &delay) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageDelay(intern->magick_wand, delay);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image delay" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::colorizeImage(ImagickPixel colorize, ImagickPixel opacity)
	Blends the fill color with each pixel in the image.
*/
PHP_METHOD(imagick, colorizeimage)
{
	PixelWand *param_wand = NULL;
	php_imagick_object *intern;
	zval *color_param, *opacity_param;
	MagickBooleanType status;
	PixelWand *color_wand, *opacity_wand;
	zend_bool color_allocated, opacity_allocated;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &color_param, &opacity_param) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (color_param, IMAGICK_CLASS, &color_allocated TSRMLS_CC);
	if (!color_wand)
		return;

	opacity_wand = php_imagick_zval_to_opacity (opacity_param, IMAGICK_CLASS, &opacity_allocated TSRMLS_CC);
	if (!opacity_wand) {
		if (color_allocated)
			color_wand = DestroyPixelWand (color_wand);

		return;
	}

	/* Colorize wants both wands to be of same instance */
	IMAGICK_CLONE_PIXELWAND(color_wand, param_wand);
	PixelSetOpacity(param_wand, PixelGetOpacity(opacity_wand));
	PixelSetAlpha(param_wand, PixelGetAlpha(opacity_wand));

	status = MagickColorizeImage(intern->magick_wand, param_wand, param_wand);
	param_wand = DestroyPixelWand(param_wand);

	if (color_allocated)
		color_wand = DestroyPixelWand (color_wand);

	if (opacity_allocated)
		opacity_wand = DestroyPixelWand (opacity_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to colorize image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::compareImageChannels(Imagick reference, int channel, int metric)
	Compares one or more image channels of an image to a reconstructed image and returns the difference image.
*/
PHP_METHOD(imagick, compareimagechannels)
{
	MagickWand *tmp_wand;
	zval *objvar;
	php_imagick_object *intern, *intern_second, *intern_return;
	long channel_type, metric_type;
	double distortion;
	zval *new_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Oll", &objvar, php_imagick_sc_entry, &channel_type, &metric_type) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	intern_second = (php_imagick_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern_second->magick_wand);

	tmp_wand = MagickCompareImageChannels(intern->magick_wand, intern_second->magick_wand, channel_type, metric_type, &distortion);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Compare image channels failed" TSRMLS_CC);
		return;
	}

	MAKE_STD_ZVAL(new_wand); 
	array_init(return_value);
	object_init_ex(new_wand, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(new_wand TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	add_next_index_zval(return_value, new_wand);
	add_next_index_double(return_value, distortion);

	return;
}
/* }}} */
#if MagickLibVersion > 0x628
/* {{{ proto Imagick Imagick::compareImageLayers(int method)
	Compares each image with the next in a sequence and returns the maximum bounding region of any pixel differences it discovers.
*/
PHP_METHOD(imagick, compareimagelayers)
{
	MagickWand *tmp_wand;
	php_imagick_object *intern, *intern_return;
	long compare_method;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &compare_method) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = (MagickWand *)MagickCompareImageLayers(intern->magick_wand, compare_method);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Compare image layers failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;
}
/* }}} */
#endif
/* {{{ proto Imagick Imagick::flattenImages()
	Merges a sequence of images.  This is useful for combining Photoshop layers into a single image.
*/
PHP_METHOD(imagick, flattenimages)
{
	php_imagick_object *intern, *intern_return;
	MagickWand *tmp_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	MagickSetFirstIterator(intern->magick_wand);

	tmp_wand = MagickFlattenImages(intern->magick_wand);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Flatten images failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto bool Imagick::flipImage()
	Creates a vertical mirror image by reflecting the pixels around the central x-axis.
*/
PHP_METHOD(imagick, flipimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickFlipImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to flip image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::flopImage()
	Creates a horizontal mirror image by reflecting the pixels around the central y-axis.
*/
PHP_METHOD(imagick, flopimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickFlopImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to flop image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::frameImage(ImagickPixel matte_color, int width, int height, int inner_bevel, int outer_bevel)
	Adds a simulated three-dimensional border around the image
*/
PHP_METHOD(imagick, frameimage)
{
	zval *param;
	php_imagick_object *intern;
	MagickBooleanType status;
	long width, height, inner_bevel, outer_bevel;
	PixelWand *color_wand;
	zend_bool allocated;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zllll", &param, &width, &height, &inner_bevel, &outer_bevel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickFrameImage(intern->magick_wand, color_wand, width, height, inner_bevel, outer_bevel);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to frame image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::fxImage(string expression[, int channel])
	Evaluate expression for each pixel in the image.
*/
PHP_METHOD(imagick, fximage)
{
	MagickWand *tmp_wand;
	php_imagick_object *intern, *intern_return;
	char *expression;
	int expression_len;
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &expression, &expression_len, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = MagickFxImageChannel(intern->magick_wand, channel, expression);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Fx image failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto bool Imagick::gammaImage(float gamma[, int channel])
	Gamma-corrects an image.  The same image viewed on different devices will have perceptual differences in the way the image's intensities are represented on the screen.  Specify individual gamma levels for the red, green, and blue channels, or adjust all three with the gamma parameter.  Values typically range from 0.8 to 2.3.
*/
PHP_METHOD(imagick, gammaimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double gamma;
	long channel = DefaultChannels;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d|l", &gamma, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGammaImageChannel(intern->magick_wand, channel, gamma);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to gamma image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::gaussianBlurImage(float radius, float sigma[, int channel])
	Blurs an image.  We convolve the image with a Gaussian operator of the given radius and standard deviation (sigma). For reasonable results, the radius should be larger than sigma.  Use a radius of 0 and selects a suitable radius for you.
*/
PHP_METHOD(imagick, gaussianblurimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double radius, sigma;
	long channel = DefaultChannels;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|l", &radius, &sigma, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickGaussianBlurImageChannel(intern->magick_wand, channel, radius, sigma);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to gaussian blur image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::compareImageChannels(int metric, int distortion)
	Compares one or more images and returns the difference image.
*/
PHP_METHOD(imagick, compareimages)
{
	MagickWand *tmp_wand;
	zval *objvar;
	php_imagick_object *intern, *intern_second, *intern_return;
	long metric_type;
	double distortion;
	zval *new_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Ol", &objvar, php_imagick_sc_entry, &metric_type) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	intern_second = (php_imagick_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern_second->magick_wand);

	MAKE_STD_ZVAL(new_wand);
	array_init(return_value);

	tmp_wand = MagickCompareImages(intern->magick_wand, intern_second->magick_wand, metric_type, &distortion);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Compare images failed" TSRMLS_CC);
		return;
	}

	object_init_ex(new_wand, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(new_wand TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	add_next_index_zval(return_value, new_wand);
	add_next_index_double(return_value, distortion);

	return;
}
/* }}} */

/* {{{ proto bool Imagick::contrastImage(bool sharpen)
	Enhances the intensity differences between the lighter and darker elements of the image.  Set sharpen to a value other than 0 to increase the image contrast otherwise the contrast is reduced.
*/
PHP_METHOD(imagick, contrastimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	zend_bool contrast;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &contrast) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickContrastImage(intern->magick_wand, contrast);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to contrast image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageDepth(int depth)
	Sets the image depth.
*/
PHP_METHOD(imagick, setimagedepth)
{
	php_imagick_object *intern;
	long depth;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &depth) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageDepth(intern->magick_wand, depth);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image depth" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageGamma(float gamma)
	Sets the image gamma.
*/
PHP_METHOD(imagick, setimagegamma)
{
	php_imagick_object *intern;
	double gamma;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &gamma) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageGamma(intern->magick_wand, gamma);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image gamma" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageIterations(int iterations)
	Sets the image iterations.
*/
PHP_METHOD(imagick, setimageiterations)
{
	php_imagick_object *intern;
	long iterations;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &iterations) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageIterations(intern->magick_wand, iterations);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image iterations" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageMatteColor(ImagickPixel matte)
	Sets the image matte color.
*/
PHP_METHOD(imagick, setimagemattecolor)
{
	zval *param;
	php_imagick_object *intern;
	MagickBooleanType status;
	PixelWand *color_wand;
	zend_bool allocated;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickSetImageMatteColor(intern->magick_wand, color_wand);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image matte color" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImagePage(int width, int height, int x, int y)
	Sets the page geometry of the image.
*/
PHP_METHOD(imagick, setimagepage)
{
	php_imagick_object *intern;
	long width, height, x, y;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &width, &height, &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImagePage(intern->magick_wand, width, height, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image page" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageResolution(float x_resolution,const float y_resolution)
	Sets the image resolution.
*/
PHP_METHOD(imagick, setimageresolution)
{
	php_imagick_object *intern;
	double x_res, y_res;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x_res, &y_res) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageResolution(intern->magick_wand, x_res, y_res);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image resolution" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageScene(int scene)
	Sets the image scene.
*/
PHP_METHOD(imagick, setimagescene)
{
	php_imagick_object *intern;
	long scene;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &scene) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageScene(intern->magick_wand, scene);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image scene" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageTicksPerSecond(int ticks_per-second)
	Sets the image ticks-per-second.
*/
PHP_METHOD(imagick, setimagetickspersecond)
{
	php_imagick_object *intern;
	long ticks_per_second;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &ticks_per_second) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageTicksPerSecond(intern->magick_wand, ticks_per_second);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image ticks per second" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageType(int image_type)
	Sets the image type.
*/
PHP_METHOD(imagick, setimagetype)
{
	php_imagick_object *intern;
	long image_type;	
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &image_type) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageType(intern->magick_wand, image_type);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image type" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageUnits(int units)
	Sets the image units of resolution.
*/
PHP_METHOD(imagick, setimageunits)
{
	php_imagick_object *intern;
	long units;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &units) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageUnits(intern->magick_wand, units);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image units" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setImageFormat(string format)
	Sets the format of a particular image in a sequence.
*/
PHP_METHOD(imagick, setimageformat)
{
	char *format;
	int format_len;
	MagickBooleanType status;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &format, &format_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageFormat(intern->magick_wand, format);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image format" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::charcoalImage(float radius, float sigma)
	Simulates a charcoal drawing.
*/
PHP_METHOD(imagick, charcoalimage)
{
	double sigma, radius;
	MagickBooleanType status;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &radius, &sigma) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickCharcoalImage(intern->magick_wand, sigma, radius);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to charcoal image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::oilPaintImage(float radius)
	Applies a special effect filter that simulates an oil painting.  Each pixel is replaced by the most frequent color occurring in a circular region defined by radius.
*/
PHP_METHOD(imagick, oilpaintimage)
{
	double radius;
	MagickBooleanType status;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &radius) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickOilPaintImage(intern->magick_wand, radius);

	/* No magick is going to happen */
	if (status == MagickFalse)
	{
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to oilpaint image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::normalizeImage([int channel])
	Enhances the contrast of a color image by adjusting the pixels color to span the entire range of colors available, Channel parameter is ignored in ImageMagick below 6.2.8
*/
PHP_METHOD(imagick, normalizeimage)
{
	MagickBooleanType status;
	php_imagick_object *intern;
	long channel;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
#if MagickLibVersion > 0x628
	status = MagickNormalizeImageChannel(intern->magick_wand, channel);
#else
	status = MagickNormalizeImage(intern->magick_wand);
#endif
	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to normalize image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::labelImage(string label)
	Adds a label to your image.
*/
PHP_METHOD(imagick, labelimage)
{
	char *label;
	int label_len;
	MagickBooleanType status;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &label, &label_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	status = MagickLabelImage(intern->magick_wand, label);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to label image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::writeImage(string filename)
	Writes an image to the specified filename
*/
PHP_METHOD(imagick, writeimage)
{
	char *filename = NULL;
	int filename_len = 0;
	zend_bool free_filename = 0;
	php_imagick_object *intern;
	struct php_imagick_file_t file = {0};
	php_imagick_rw_result_t rc;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s!", &filename, &filename_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	if (!filename) {
		filename = MagickGetImageFilename(intern->magick_wand);

		if (!filename) {
			php_imagick_throw_exception(IMAGICK_CLASS, "No image filename specified" TSRMLS_CC);
			return;
		}
		filename_len = strlen(filename);
		free_filename = 1;
	}

	if (!filename_len) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Can not use empty string as a filename" TSRMLS_CC);
		return;
	}

	if (!php_imagick_file_init(&file, filename, filename_len TSRMLS_CC)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The filename is too long" TSRMLS_CC);
		return;
	}
	rc = php_imagick_write_file(intern, &file, ImagickWriteImage, 0 TSRMLS_CC);
	php_imagick_file_deinit(&file);

	if (rc != IMAGICK_RW_OK) {
		php_imagick_rw_fail_to_exception (intern->magick_wand, rc, filename TSRMLS_CC);
		if (free_filename) {
			IMAGICK_FREE_MEMORY(char *, filename);
		}
		return;
	}

	if (free_filename) {
		IMAGICK_FREE_MEMORY(char *, filename);
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::writeImages(string filename, bool adjoin)
	Writes an image or image sequence.
*/
PHP_METHOD(imagick, writeimages)
{
	char *filename;
	zend_bool adjoin;
	int filename_len;
	php_imagick_object *intern;
	struct php_imagick_file_t file = {0};
	php_imagick_rw_result_t rc;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sb", &filename, &filename_len, &adjoin) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	if (!filename_len) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Can not use empty string as a filename" TSRMLS_CC);
		return;
	}

	if (!php_imagick_file_init(&file, filename, filename_len TSRMLS_CC)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "The filename is too long" TSRMLS_CC);
		return;
	}
	rc = php_imagick_write_file(intern, &file, ImagickWriteImages, adjoin TSRMLS_CC);
	php_imagick_file_deinit(&file);

	if (rc != IMAGICK_RW_OK) {
		php_imagick_rw_fail_to_exception (intern->magick_wand, rc, filename TSRMLS_CC);
		return;
	}
	RETURN_TRUE;

}
/* }}} */

/* {{{ proto bool Imagick::drawImage(ImagickDraw drawing_wand)
	Renders the ImagickDrawing object on the current image.
*/
PHP_METHOD(imagick, drawimage)
{
	zval *objvar;
	php_imagick_object *intern;
	MagickBooleanType status;
	php_imagickdraw_object *internd;
	char *old_locale = NULL, *buffer = NULL;
	zend_bool restore = 0;

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &objvar, php_imagickdraw_sc_entry) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(objvar TSRMLS_CC);

	IMAGICK_SET_LOCALE(old_locale, buffer, restore);
	status = MagickDrawImage(intern->magick_wand, internd->drawing_wand);
	IMAGICK_RESTORE_LOCALE(old_locale, restore);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to draw image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::annotateImage(ImagickDraw drawing_wand, float x, float y, float angle, string text)
	Annotates an image with text.
*/
PHP_METHOD(imagick, annotateimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	php_imagickdraw_object *internd;
	double x, y, angle;
	char *text;
	int text_len;
	zval *objvar;
#if MagickLibVersion < 0x632
	char *font;
#endif

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Oddds", &objvar, php_imagickdraw_sc_entry, &x, &y, &angle, &text, &text_len) == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);	

	internd = (php_imagickdraw_object *)zend_object_store_get_object(objvar TSRMLS_CC);

#if MagickLibVersion < 0x632
	font = DrawGetFont(internd->drawing_wand);

	/* Fixes PECL Bug #11328 */
	if (!font) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Font needs to be set before annotating an image" TSRMLS_CC);
		return;
	}
#endif

	status = MagickAnnotateImage(intern->magick_wand, internd->drawing_wand, x, y, angle, text);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to annotate image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::getImageCompressionQuality(int quality)
        Gets the image compression quality 
*/
PHP_METHOD(imagick, getimagecompressionquality)
{
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETVAL_LONG(MagickGetImageCompressionQuality(intern->magick_wand));
}
/* }}} */

/* {{{ proto bool Imagick::setImageCompressionQuality(int quality)
	Sets the image compression quality
*/
PHP_METHOD(imagick, setimagecompressionquality)
{
	long quality;
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &quality) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSetImageCompressionQuality(intern->magick_wand, quality);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set image compression quality" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::compositeImage(Imagick composite_wand, int compose, int x, int y[, int channel] )
	Composite one image onto another at the specified offset. Channel parameter is ignored in ImageMagick below 6.2.8
*/
PHP_METHOD(imagick, compositeimage)
{
	zval *objvar;
	php_imagick_object *intern;
	php_imagick_object *intern_second;
	long x, y;
	long composite_id = 0;
	long channel = DefaultChannels;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Olll|l", &objvar, php_imagick_sc_entry, &composite_id, &x, &y, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	intern_second = (php_imagick_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern_second->magick_wand);

#if MagickLibVersion > 0x628
	MagickCompositeImageChannel(intern->magick_wand, channel, intern_second->magick_wand, composite_id, x, y);
#else
	MagickCompositeImage(intern->magick_wand, intern_second->magick_wand, composite_id, x, y);
#endif

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::modulateImage(float brightness, float saturation, float hue)
	Lets you control the brightness, saturation, and hue of an image.  Hue is the percentage of absolute rotation from the current position.  For example 50 results in a counter-clockwise rotation of 90 degrees, 150 results in a clockwise rotation of 90 degrees, with 0 and 200 both resulting in a rotation of 180 degrees.
*/
PHP_METHOD(imagick, modulateimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double brightness, saturation, hue;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd", &brightness, &saturation, &hue) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickModulateImage(intern->magick_wand, brightness, saturation, hue);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to modulate image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::addNoiseImage(int noise_type[, int channel])
	Adds random noise to the image. Channel parameter is ignored in ImageMagick below 6.2.8
*/
PHP_METHOD(imagick, addnoiseimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long noise;
	long channel = DefaultChannels;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &noise, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

#if MagickLibVersion > 0x628
	status = MagickAddNoiseImageChannel(intern->magick_wand, channel, noise);
#else
	status = MagickAddNoiseImage(intern->magick_wand, noise);
#endif

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to add image noise" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::montageImage(ImagickDraw drawing_wand, string tile_geometry, string thumbnail_geometry, int mode, string frame)
	Creates a composite image by combining several separate images. The images are tiled on the composite image with the name of the image optionally appearing just below the individual tile.
*/

PHP_METHOD(imagick, montageimage)
{
	MagickWand *tmp_wand;
	zval *objvar;
	php_imagick_object *intern, *intern_return;
	php_imagickdraw_object *internd;
	char *tile_geometry, *thumbnail_geometry, *frame;
	int tile_geometry_len, thumbnail_geometry_len, frame_len;
	long montage_mode = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Ossls", &objvar, php_imagickdraw_sc_entry,
		&tile_geometry, &tile_geometry_len,
		&thumbnail_geometry, &thumbnail_geometry_len,
		&montage_mode,
		&frame, &frame_len) == FAILURE)
	{
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	internd = (php_imagickdraw_object *)zend_object_store_get_object(objvar TSRMLS_CC);

	tmp_wand = MagickMontageImage(intern->magick_wand, internd->drawing_wand, tile_geometry, thumbnail_geometry, montage_mode, frame);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Montage image failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto bool Imagick::affineTransformImage(ImagickDraw drawing_wand)
	Transforms an image as dictated by the affine matrix of the drawing wand.
*/
PHP_METHOD(imagick, affinetransformimage)
{
	zval *objvar;
	php_imagick_object *intern;
	php_imagickdraw_object *internd;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &objvar, php_imagickdraw_sc_entry) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	internd = (php_imagickdraw_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	status = MagickAffineTransformImage(intern->magick_wand, internd->drawing_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to affine transform image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::averageImages()
	Average a set of images.
*/
PHP_METHOD(imagick, averageimages)
{
	MagickWand *tmp_wand;
	php_imagick_object *intern, *intern_return;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = MagickAverageImages(intern->magick_wand);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Averaging images failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);
	return;
}
/* }}} */

/* {{{ proto bool Imagick::borderImage(ImagickPixel bordercolor, int width, int height)
	Surrounds the image with a border of the color defined by the bordercolor pixel wand.
*/
PHP_METHOD(imagick, borderimage)
{
	zval *param;
	php_imagick_object *intern;
	MagickBooleanType status;
	long width, height;
	PixelWand *color_wand;
	zend_bool allocated;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zll", &param, &width, &height) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickBorderImage(intern->magick_wand, color_wand, width, height);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to border image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::thresholdImage(float threshold[, int channel] )
	Changes the value of individual pixels based on the intensity of each pixel compared to threshold.  The result is a high-contrast, two color image.
*/
PHP_METHOD(imagick, thresholdimage)
{
	php_imagick_object *intern;
	double threshold;
	MagickBooleanType status;
	long channel = DefaultChannels;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d|l", &threshold, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickThresholdImageChannel(intern->magick_wand, channel, threshold);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to threshold image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::adaptiveThresholdImage(int width, int height, int offset)
	Selects an individual threshold for each pixel based on the range of intensity values in its local neighborhood.  This allows for thresholding of an image whose global intensity histogram doesn't contain distinctive peaks.
*/
PHP_METHOD(imagick, adaptivethresholdimage)
{
	php_imagick_object *intern;
	long width, height, offset;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll", &width, &height, &offset) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickAdaptiveThresholdImage(intern->magick_wand, width, height, offset);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to adaptive threshold image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::sharpenImage(float radius, float sigma[, int channel])
	Sharpens an image.  We convolve the image with a Gaussian operator of the given radius and standard deviation (sigma). For reasonable results, the radius should be larger than sigma.  Use a radius of 0 and selects a suitable radius for you.
*/
PHP_METHOD(imagick, sharpenimage)
{
	double sigma, radius;
	MagickBooleanType status;
	php_imagick_object *intern;
	long channel = DefaultChannels;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|l", &radius, &sigma, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSharpenImageChannel(intern->magick_wand, channel, radius, sigma);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to sharpen image" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::shaveImage(int columns, int rows)
	Shaves pixels from the image edges.  It allocates the memory necessary for the new Image structure and returns a pointer to the new image.
*/
PHP_METHOD(imagick, shaveimage)
{
	php_imagick_object *intern;
	long columns, rows;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &columns, &rows) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickShaveImage(intern->magick_wand, columns, rows);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to shave image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::shearImage(ImagickPixel background, float x_shear, float y_shear)
	Slides one edge of an image along the X or Y axis
*/
PHP_METHOD(imagick, shearimage)
{
	zval *param;
	php_imagick_object *intern;
	double x_shear, y_shear;
	MagickBooleanType status;
	PixelWand *color_wand;
	zend_bool allocated;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zdd", &param, &x_shear, &y_shear) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickShearImage(intern->magick_wand, color_wand, x_shear, y_shear);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to shear image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::spliceImage(int width, int height, int x, int y)
	Splices a solid color into the image.
*/
PHP_METHOD(imagick, spliceimage)
{
	long width, height, x, y;
	php_imagick_object *intern;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &width, &height, &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSpliceImage(intern->magick_wand, width, height, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to splice image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::steganoImage(Imagick watermark_wand, int offset)
	Hides a digital watermark within the image. Recover the hidden watermark later to prove that the authenticity of an image.  Offset defines the start position within the image to hide the watermark.
*/
PHP_METHOD(imagick, steganoimage)
{
	zval *objvar;
	php_imagick_object *intern, *intern_second, *intern_return;
	long offset;
	MagickWand *tmp_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Ol", &objvar, php_imagick_sc_entry, &offset) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	intern_second = (php_imagick_object *)zend_object_store_get_object(objvar TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern_second->magick_wand);

	tmp_wand = MagickSteganoImage(intern->magick_wand, intern_second->magick_wand, offset);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Stegano image failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);
	return;
}
/* }}} */

/* {{{ proto Imagick Imagick::clone()
	Makes an exact copy of the Imagick object.
*/
PHP_METHOD(imagick, clone)
{
	php_imagick_object *intern, *intern_return;
	MagickWand *tmp_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	IMAGICK_METHOD_DEPRECATED("Imagick", "clone");
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	tmp_wand = CloneMagickWand(intern->magick_wand);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Cloning Imagick object failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);
	return;
}
/* }}} */

/* {{{ proto bool Imagick::rotateImage(ImagickPixel background, float degrees)
	Rotates an image the specified number of degrees. Empty triangles left over from rotating the image are filled with the background color.
*/
PHP_METHOD(imagick, rotateimage)
{
	zval *param;
	php_imagick_object *intern;
	double degrees;
	MagickBooleanType status;
	PixelWand *color_wand;
	zend_bool allocated;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zd", &param, &degrees) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickRotateImage(intern->magick_wand, color_wand, degrees);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to rotate image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::sampleImage(int columns, int rows)
	Scales an image to the desired dimensions with pixel sampling.  Unlike other scaling methods, this method does not introduce any additional color into the scaled image.
*/
PHP_METHOD(imagick, sampleimage)
{
	php_imagick_object *intern;
	long columns, rows;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &columns, &rows) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSampleImage(intern->magick_wand, columns, rows);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to sample image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::solarizeImage(float threshold)
	Applies a special effect to the image, similar to the effect achieved in a photo darkroom by selectively exposing areas of photo sensitive paper to light.  Threshold ranges from 0 to QuantumRange and is a measure of the extent of the solarization.
*/
PHP_METHOD(imagick, solarizeimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long threshold;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &threshold) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickSolarizeImage(intern->magick_wand, threshold);

	/* No magick is going to happen */
	if (status == MagickFalse)
	{
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to solarize image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::shadowImage(float opacity, float sigma, int x, int y)
	Simulates an image shadow.
*/
PHP_METHOD(imagick, shadowimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double opacity, sigma;
	long x, y;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddll", &opacity, &sigma, &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickShadowImage(intern->magick_wand, opacity, sigma, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to shadow image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::motionBlurImage(float radius, float sigma, float angle[, int CHANNEL])
	Simulates motion blur.  We convolve the image with a Gaussian operator of the given radius and standard deviation (sigma). For reasonable results, radius should be larger than sigma.  Use a radius of 0 and MotionBlurImage() selects a suitable radius for you. Angle gives the angle of the blurring motion.
*/
PHP_METHOD(imagick, motionblurimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double radius, sigma, angle;
	long channel = DefaultChannels;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd|l", &radius, &sigma, &angle, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

#if MagickLibVersion > 0x643
	status = MagickMotionBlurImageChannel(intern->magick_wand, channel, radius, sigma, angle);
#else
	status = MagickMotionBlurImage(intern->magick_wand, radius, sigma, angle);
#endif

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to motion blur image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;


}
/* }}} */

/* {{{ proto Imagick Imagick::mosaicImages()
	Inlays an image sequence to form a single coherent picture.  It returns a wand with each image in the sequence composited at the location defined by the page offset of the image.
*/
PHP_METHOD(imagick, mosaicimages)
{
	MagickWand *tmp_wand = NULL;
	php_imagick_object *intern, *intern_return;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	MagickSetFirstIterator(intern->magick_wand);
	tmp_wand = MagickMosaicImages(intern->magick_wand);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Mosaic image failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;

}
/* }}} */

/* {{{ proto Imagick Imagick::morphImages(int number_frames)
	Method morphs a set of images.  Both the image pixels and size are linearly interpolated to give the appearance of a meta-morphosis from one image to the next.
*/
PHP_METHOD(imagick, morphimages)
{
	MagickWand *tmp_wand;
	php_imagick_object *intern, *intern_return;
	long frames;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &frames) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	tmp_wand = MagickMorphImages(intern->magick_wand, frames);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Morphing images failed" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);

	return;

}
/* }}} */

/* {{{ proto bool Imagick::minifyImage()
	Is a convenience method that scales an image proportionally to one-half its original size
*/
PHP_METHOD(imagick, minifyimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickMinifyImage(intern->magick_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to minify image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;

}
/* }}} */

/* {{{ proto bool Imagick::posterizeImage(int levels, bool dither)
	Reduces the image to a limited number of color level.
*/
PHP_METHOD(imagick, posterizeimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long levels;
	zend_bool dither;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lb", &levels, &dither) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickPosterizeImage(intern->magick_wand, levels, dither);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to posterize image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;

}
/* }}} */

/* {{{ proto bool Imagick::radialBlurImage(float angle[, int channel])
	Radial blurs an image.
*/
PHP_METHOD(imagick, radialblurimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double angle;
	long channel = DefaultChannels;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d|l", &angle, &channel) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickRadialBlurImageChannel(intern->magick_wand, channel, angle);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to radial blur image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::raiseImage(int width, int height, int x, int y, bool raise)
	Creates a simulated three-dimensional button-like effect by lightening and darkening the edges of the image.  Members width and height of raise_info define the width of the vertical and horizontal edge of the effect.
*/
PHP_METHOD(imagick, raiseimage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long width, height, x, y;
	zend_bool raise;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llllb", &width, &height, &x, &y, &raise) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	status = MagickRaiseImage(intern->magick_wand, width, height, x, y, raise);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to raise image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::blackThresholdImage(ImagickPixel threshold)
	Is like MagickThresholdImage() but  forces all pixels below the threshold into black while leaving all pixels above the threshold unchanged.
*/
PHP_METHOD(imagick, blackthresholdimage)
{
	php_imagick_object *intern;
	zval *param;
	MagickBooleanType status;
	PixelWand *color_wand;
	zend_bool allocated;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickBlackThresholdImage(intern->magick_wand, color_wand);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to black threshold image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::resampleImage(float x_resolution, float y_resolution, int filter, float blur)
	Resample image to desired resolution.
*/
PHP_METHOD(imagick, resampleimage)
{
	double xRes, yRes, blur;
	long filter = 0;
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddld", &xRes, &yRes, &filter, &blur) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	status = MagickResampleImage(intern->magick_wand, xRes, yRes, filter, blur);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to resample image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::resizeImage(int width, int height, int filter, float blur[, bool bestfit = false])
	Scales an image to the desired dimensions with one of these filters:
*/
PHP_METHOD(imagick, resizeimage)
{
	double blur;
	long width, height, new_width, new_height, filter = 0;
	php_imagick_object *intern;
	MagickBooleanType status;
	zend_bool bestfit = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llld|b", &width, &height, &filter, &blur, &bestfit) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	
	if (!php_imagick_thumbnail_dimensions(intern->magick_wand, bestfit, width, height, &new_width, &new_height)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Invalid image geometry" TSRMLS_CC);
		return;
	}

	status = MagickResizeImage(intern->magick_wand, new_width, new_height, filter, blur);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to resize image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::rollImage(int x, int y)
	Offsets an image as defined by x and y.
*/
PHP_METHOD(imagick, rollimage)
{
	long x, y;
	php_imagick_object *intern;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	status = MagickRollImage(intern->magick_wand, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to roll image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto Imagick Imagick::appendImages(bool stack)
	Append a set of images.
*/
PHP_METHOD(imagick, appendimages)
{
	php_imagick_object *intern, *intern_return;
	MagickWand *tmp_wand;
	zend_bool stack;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &stack) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);
	tmp_wand = MagickAppendImages(intern->magick_wand, stack);

	if (tmp_wand == (MagickWand *)NULL) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to append images" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagick_sc_entry);
	intern_return = (php_imagick_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICK_REPLACE_MAGICKWAND(intern_return, tmp_wand);
	return;
}
/* }}} */

/* {{{ proto bool Imagick::whiteThresholdImage(ImagickPixel threshold)
	Is like ThresholdImage() but  force all pixels above the threshold into white while leaving all pixels below the threshold unchanged.
*/
PHP_METHOD(imagick, whitethresholdimage)
{
	php_imagick_object *intern;
	zval *param;
	MagickBooleanType status;
	PixelWand *color_wand;
	zend_bool allocated;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = MagickWhiteThresholdImage(intern->magick_wand, color_wand);

	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to white threshold image" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto ImagickPixelIterator Imagick::getPixelIterator()
	Returns a MagickPixelIterator.
*/
PHP_METHOD(imagick, getpixeliterator)
{
	php_imagick_object *intern;
	zval retval, *method_array;
	zval *args[1];
	zval *tmp_object;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	MAKE_STD_ZVAL(tmp_object);

	object_init_ex(tmp_object, php_imagickpixeliterator_sc_entry);

	MAKE_STD_ZVAL(method_array);
	array_init(method_array);

	add_next_index_zval(method_array, tmp_object);
	add_next_index_string(method_array, "newpixeliterator", 1);

	args[0] = getThis();
	call_user_function(EG(function_table), NULL, method_array, &retval, 1, args TSRMLS_CC);

	*return_value = *tmp_object;
	zval_copy_ctor(return_value);

	zval_dtor(method_array);
	FREE_ZVAL(method_array);

	return;
}
/* }}} */

/* {{{ proto ImagickPixelIterator Imagick::getPixelRegionIterator(long x, long y, long columns, long rows)
	Returns a subset of pixels in a MagickPixelIterator object.
*/
PHP_METHOD(imagick, getpixelregioniterator)
{
	zval *object;
	php_imagick_object *intern;
	zval retval, *method_array;
	zval *args[5];
	zval *tmp_object;
	zval *x, *y, *columns, *rows;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzz", &x, &y, &columns, &rows) == FAILURE) {
		return;
	}

	object = getThis();
	intern = (php_imagick_object *)zend_object_store_get_object(object TSRMLS_CC);

	IMAGICK_ENSURE_NOT_EMPTY(intern->magick_wand);

	MAKE_STD_ZVAL(tmp_object);

	object_init_ex(tmp_object, php_imagickpixeliterator_sc_entry);

	MAKE_STD_ZVAL(method_array);
	array_init(method_array);

	add_next_index_zval(method_array, tmp_object);
	add_next_index_string(method_array, "newpixelregioniterator", 1);

	args[0] = object;
	args[1] = x;
	args[2] = y;
	args[3] = columns;
	args[4] = rows;
	call_user_function(EG(function_table), NULL, method_array, &retval, 5, args TSRMLS_CC);

	*return_value = *tmp_object;
	zval_copy_ctor(return_value);

	zval_dtor(method_array);
	FREE_ZVAL(method_array);

	return;

}
/* }}} */

/* {{{ proto int Imagick::getCompression()
	Gets the wand compression type.
*/
PHP_METHOD(imagick, getcompression)
{
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
		
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETVAL_LONG(MagickGetCompression(intern->magick_wand));
}
/* }}} */

/* {{{ proto int Imagick::getCompressionQuality()
	Gets the wand compression quality.
*/
PHP_METHOD(imagick, getcompressionquality)
{
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETVAL_LONG(MagickGetCompressionQuality(intern->magick_wand));
}
/* }}} */

/* {{{ proto string Imagick::getCopyright()
	Returns the ImageMagick API copyright as a string constant.
*/
PHP_METHOD(imagick, getcopyright)
{
	char *copyright;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	copyright = (char *)MagickGetCopyright();
	ZVAL_STRING(return_value, copyright, 1);
	return;
}
/* }}} */

/* {{{ proto string Imagick::getFilename()
	Returns the filename associated with an image sequence.
*/
PHP_METHOD(imagick, getfilename)
{
	php_imagick_object *intern;
	char *filename;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	filename = (char *)MagickGetFilename(intern->magick_wand);
	
	if (filename) {
		ZVAL_STRING(return_value, filename, 1);
		IMAGICK_FREE_MEMORY(char *, filename);
	}
	return;
}
/* }}} */

/* {{{ proto string Imagick::getFormat()
	Returns the format of the Imagick object.
*/
PHP_METHOD(imagick, getformat)
{
	php_imagick_object *intern;
	char *format;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	format = (char *)MagickGetFormat(intern->magick_wand);
	
	if (format) {
		ZVAL_STRING(return_value, format, 1);
		IMAGICK_FREE_MEMORY(char *, format);
	}
	return;
}
/* }}} */

/* {{{ proto string Imagick::getHomeURL()
	Returns the ImageMagick home URL.
*/
PHP_METHOD(imagick, gethomeurl)
{
	char *home_url;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	home_url = (char *)MagickGetHomeURL();
	if (home_url) {
		ZVAL_STRING(return_value, home_url, 1);
		IMAGICK_FREE_MEMORY(char *, home_url);
	}
	return;
}
/* }}} */

/* {{{ proto int Imagick::getInterlaceScheme()
	Gets the wand interlace scheme.
*/
PHP_METHOD(imagick, getinterlacescheme)
{
	php_imagick_object *intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETVAL_LONG(MagickGetInterlaceScheme(intern->magick_wand));
}
/* }}} */

/* {{{ proto string Imagick::getOption(string key)
	Returns a value associated with a wand and the specified key. Use MagickRelinquishMemory() to free the value when you are finished with it.
*/
PHP_METHOD(imagick, getoption)
{
	php_imagick_object *intern;
	char *key, *value;
	int key_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	value = MagickGetOption(intern->magick_wand, key);

	if (value) {	
		ZVAL_STRING(return_value, value, 1);
		IMAGICK_FREE_MEMORY(char *, value);
	}
	return;
}
/* }}} */

/* {{{ proto string Imagick::getPackageName()
	Returns the ImageMagick package name as a string constant.
*/
PHP_METHOD(imagick, getpackagename)
{
	char *package_name;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	package_name = (char *)MagickGetPackageName();
	ZVAL_STRING(return_value, package_name, 1);
	return;
}
/* }}} */

/* {{{ proto array Imagick::getPage()
	Returns the page geometry associated with the Imagick object in an associative array with the keys "width", "height", "x", and "y".
*/
PHP_METHOD(imagick, getpage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	unsigned long width, height;
	long x, y;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickGetPage(intern->magick_wand, &width, &height, &x, &y);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get page" TSRMLS_CC);
		return;
	}

	array_init(return_value);
	add_assoc_long(return_value, "width", width);
	add_assoc_long(return_value, "height", height);
	add_assoc_long(return_value, "x", x);
	add_assoc_long(return_value, "y", y);

	return;
}
/* }}} */

/* {{{ proto array Imagick::getQuantumDepth()
	Returns the ImageMagick quantum depth as a string constant.
*/
PHP_METHOD(imagick, getquantumdepth)
{
	char *quantum_depth;
	unsigned long depth;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	quantum_depth = (char *)MagickGetQuantumDepth(&depth);

	array_init(return_value);
	add_assoc_long(return_value, "quantumDepthLong", depth);
	add_assoc_string(return_value, "quantumDepthString", quantum_depth, 1);

	return;
}
/* }}} */

/* {{{ proto array Imagick::getQuantumRange()
	Returns the ImageMagick quantum range as a string constant.
*/
PHP_METHOD(imagick, getquantumrange)
{
	char *quantum_range;
	unsigned long range;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	quantum_range = (char *)MagickGetQuantumRange(&range);
	array_init(return_value);

	add_assoc_long(return_value, "quantumRangeLong", range);
	add_assoc_string(return_value, "quantumRangeString", quantum_range, 1);
	return;
}
/* }}} */

/* {{{ proto string Imagick::getReleaseDate()
	Returns the ImageMagick release date as a string constant.
*/
PHP_METHOD(imagick, getreleasedate)
{
	char *release_date;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	release_date = (char *)MagickGetReleaseDate();
	ZVAL_STRING(return_value, release_date, 1);
	return;
}
/* }}} */

/* {{{ proto int Imagick::getResource(int type)
	Returns the specified resource in megabytes.
*/
PHP_METHOD(imagick, getresource)
{
	long resource_type;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &resource_type) == FAILURE) {
		return;
	}

	RETVAL_LONG(MagickGetResource(resource_type));
}
/* }}} */

/* {{{ proto Imagick Imagick::getResourceLimit(int type)
	Returns the specified resource limit in megabytes.
*/
PHP_METHOD(imagick, getresourcelimit)
{
	long resource_type;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &resource_type) == FAILURE) {
		return;
	}

	RETVAL_LONG(MagickGetResourceLimit(resource_type));
}
/* }}} */

/* {{{ proto array Imagick::getSamplingFactors()
	Gets the horizontal and vertical sampling factor.
*/
PHP_METHOD(imagick, getsamplingfactors)
{
	php_imagick_object *intern;
	double *sampling_factors;
	unsigned long number_factors = 0, i;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	sampling_factors = (double *)MagickGetSamplingFactors(intern->magick_wand, &number_factors);

	array_init(return_value);

	for (i = 0 ; i < number_factors; i++) {
		add_next_index_double(return_value, sampling_factors[i]);
	}

	return;
}
/* }}} */

/* {{{ proto array Imagick::getSize()
	Returns the size associated with the Imagick object as an array with the keys "columns" and "rows".
*/
PHP_METHOD(imagick, getsize)
{
	php_imagick_object *intern;
	unsigned long columns, rows;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickGetSize(intern->magick_wand, &columns, &rows);

	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to get size" TSRMLS_CC);
		return;
	}

	array_init(return_value);
	add_assoc_long(return_value, "columns", columns);
	add_assoc_long(return_value, "rows", rows);

	return;
}
/* }}} */

/* {{{ proto array Imagick::getVersion()
	Returns the ImageMagick API version as a string constant and as a number.
*/
PHP_METHOD(imagick, getversion)
{
	char *version_string;
	size_t version_number;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	version_string = (char *)MagickGetVersion(&version_number);
	array_init(return_value);

	add_assoc_long(return_value, "versionNumber", (long) version_number);
	add_assoc_string(return_value, "versionString", version_string, 1);
	return;
}
/* }}} */

/* {{{ proto bool Imagick::setBackgroundColor(ImagickPixel background)
	Sets the wand background color.
*/
PHP_METHOD(imagick, setbackgroundcolor)
{
	zval *param;
	php_imagick_object *intern;
	MagickBooleanType status;
	PixelWand *background_wand;
	zend_bool allocated = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	background_wand = php_imagick_zval_to_pixelwand (param, IMAGICK_CLASS, &allocated TSRMLS_CC);
	if (!background_wand)
		return;

	status = MagickSetBackgroundColor(intern->magick_wand, background_wand);

	if (allocated)
		background_wand = DestroyPixelWand (background_wand);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set background color" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setCompression(COMPRESSIONTYPE compression)
	Sets the wand compression type.
*/
PHP_METHOD(imagick, setcompression)
{
	php_imagick_object *intern;
	long compression;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &compression) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetCompression(intern->magick_wand, compression);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set compression" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setCompressionQuality(int quality)
	Sets the wand compression quality.
*/
PHP_METHOD(imagick, setcompressionquality)
{
	php_imagick_object *intern;
	long quality;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &quality) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetCompressionQuality(intern->magick_wand, quality);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set compression quality" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setFilename(string filename)
	Sets the filename before you read or write an image file.
*/
PHP_METHOD(imagick, setfilename)
{
	php_imagick_object *intern;
	char *filename;
	int filename_len;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filename, &filename_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetFilename(intern->magick_wand, filename);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set filename" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setFormat(string format)
	Sets the format of the Imagick object.
*/
PHP_METHOD(imagick, setformat)
{
	php_imagick_object *intern;
	char *format;
	int format_len;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &format, &format_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetFormat(intern->magick_wand, format);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set format" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setInterlaceScheme(INTERLACETYPE interlace_scheme)
	Sets the image compression.
*/
PHP_METHOD(imagick, setinterlacescheme)
{
	php_imagick_object *intern;
	long schema;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &schema) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetInterlaceScheme(intern->magick_wand, schema);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set interlace scheme" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setOption(string key, string value)
	Associates one or options with the wand (.e.g MagickSetOption(wand,"jpeg:perserve","yes")).
*/
PHP_METHOD(imagick, setoption)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	char *key, *value;
	int key_len, value_len;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len, &value, &value_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetOption(intern->magick_wand, key, value);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set option" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setPage(int width, int height, int x, int y)
	Sets the page geometry of the Imagick object.
*/
PHP_METHOD(imagick, setpage)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	long width, height, x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &width, &height, &x, &y) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetPage(intern->magick_wand, width, height, x, y);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set page" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

PHP_METHOD(imagick, setimageprogressmonitor)
{
	char *filename;
	int filename_len;
	php_imagick_object *intern;
	php_imagick_rw_result_t rc;

	if (!IMAGICK_G(progress_monitor)) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Progress monitoring is disabled in ini-settings" TSRMLS_CC);
		return;
	}

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filename, &filename_len) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if ((rc = php_imagick_file_access_check (filename TSRMLS_CC)) != IMAGICK_RW_OK) {
		php_imagick_rw_fail_to_exception (intern->magick_wand, rc, filename TSRMLS_CC);
		return;
	}

	if (intern->progress_monitor_name) {
		efree(intern->progress_monitor_name);
	}

	intern->progress_monitor_name = estrdup(filename);
	MagickSetImageProgressMonitor(intern->magick_wand, php_imagick_progress_monitor, intern);
	RETURN_TRUE;
}

/* {{{ proto bool Imagick::setResourceLimit(RESOURCETYPE type, int limit)
	Sets the limit for a particular resource in megabytes.
*/
PHP_METHOD(imagick, setresourcelimit)
{
	MagickBooleanType status;
	long type, limit;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &type, &limit) == FAILURE) {
		return;
	}
	
	status = MagickSetResourceLimit(type, limit);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Unable to set resource limit" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setResolution(float x_resolution, float y_resolution)
	Sets the image resolution.
*/
PHP_METHOD(imagick, setresolution)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	double x_resolution, y_resolution;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x_resolution, &y_resolution) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetResolution(intern->magick_wand, x_resolution, y_resolution);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set resolution" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setSamplingFactors(array factors)
	Sets the image sampling factors.
*/
PHP_METHOD(imagick, setsamplingfactors)
{
	php_imagick_object *intern;
	MagickBooleanType status;
	zval *factors;
	double *double_array;
	long elements = 0;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &factors) == FAILURE) {
		return;
	}

	double_array = php_imagick_zval_to_double_array(factors, &elements TSRMLS_CC);

	if (!double_array) {
		php_imagick_throw_exception(IMAGICK_CLASS, "Can't read array" TSRMLS_CC);
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	status = MagickSetSamplingFactors(intern->magick_wand, elements, double_array);
	efree(double_array);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set sampling factors" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setSize(int columns, int rows)
	Sets the size of the Imagick object.  Set it before you read a raw image format such as RGB, GRAY, or CMYK.
*/
PHP_METHOD(imagick, setsize)
{
	php_imagick_object *intern;
	long columns, rows;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &columns, &rows) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetSize(intern->magick_wand, columns, rows);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set size" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::setType(IMAGETYPE image_type)
	Sets the image type attribute.
*/
PHP_METHOD(imagick, settype)
{
	php_imagick_object *intern;
	long type;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &type) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = MagickSetType(intern->magick_wand, type);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		php_imagick_convert_imagick_exception(intern->magick_wand, "Unable to set type" TSRMLS_CC);
		return;
	}
	RETURN_TRUE;
}
/* }}} */

/* end of Imagick */
