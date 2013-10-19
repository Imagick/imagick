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

#if MagickLibVersion > 0x628
/* {{{ proto array ImagickPixel::getHSL()
	Returns the normalized HSL color of the pixel wand in an array with the keys "hue", "saturation", and "luminosity".
*/
PHP_METHOD(imagickpixel, gethsl)
{
	php_imagickpixel_object *internp;
	double hue, saturation, luminosity;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	
	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	PixelGetHSL(internp->pixel_wand, &hue, &saturation, &luminosity);

	array_init(return_value);
	add_assoc_double(return_value, "hue", hue);
	add_assoc_double(return_value, "saturation", saturation);
	add_assoc_double(return_value, "luminosity", luminosity);
	return;
}
/* }}} */

/* {{{ proto bool ImagickPixel::setHSL(float hue, float saturation, float luminosity)
	Sets the normalized HSL color of the pixel wand.
*/
PHP_METHOD(imagickpixel, sethsl)
{
	php_imagickpixel_object *internp;
	double hue, saturation, luminosity;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd", &hue, &saturation, &luminosity) == FAILURE) {
		return;
	}

	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	PixelSetHSL(internp->pixel_wand, hue, saturation, luminosity);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto int ImagickPixel::getColorValueQuantum(int color)
	Gets the quantum color of the ImagickPixel
*/
PHP_METHOD(imagickpixel, getcolorvaluequantum)
{
	php_imagickpixel_object *internp;
	long color, color_value;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &color) == FAILURE) {
		return;
	}
	
	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	switch (color) {

		case PHP_IMAGICK_COLOR_BLACK:
			color_value = PixelGetBlackQuantum(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_BLUE:
			color_value = PixelGetBlueQuantum(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_CYAN:
			color_value = PixelGetCyanQuantum(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_GREEN:
			color_value = PixelGetGreenQuantum(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_RED:
			color_value = PixelGetRedQuantum(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_YELLOW:
			color_value = PixelGetYellowQuantum(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_MAGENTA:
			color_value = PixelGetMagentaQuantum(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_OPACITY:
			color_value = PixelGetOpacityQuantum(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_ALPHA:
			color_value = PixelGetAlphaQuantum(internp->pixel_wand);
		break;

		default:
			php_imagick_throw_exception (IMAGICKPIXEL_CLASS, "Unknown color type" TSRMLS_CC);
			return;
		break;
	}
	RETVAL_LONG(color_value);
}
/* }}} */

/* {{{ proto bool ImagickPixel::setColorValueQuantum(int color, float value)
	Sets the quantum color of the ImagickPixel.
*/
PHP_METHOD(imagickpixel, setcolorvaluequantum)
{
	php_imagickpixel_object *internp;
	long color, color_value;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &color, &color_value) == FAILURE) {
		return;
	}
	
	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	switch (color) {

		case PHP_IMAGICK_COLOR_BLACK:
			PixelSetBlackQuantum(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_BLUE:
			PixelSetBlueQuantum(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_CYAN:
			PixelSetCyanQuantum(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_GREEN:
			PixelSetGreenQuantum(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_RED:
			PixelSetRedQuantum(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_YELLOW:
			PixelSetYellowQuantum(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_MAGENTA:
			PixelSetMagentaQuantum(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_OPACITY:
			PixelSetOpacityQuantum(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_ALPHA:
			PixelSetAlphaQuantum(internp->pixel_wand, color_value);
		break;

		default:
			php_imagick_throw_exception (IMAGICKPIXEL_CLASS, "Unknown color type" TSRMLS_CC);
			return;
		break;
	}
	RETVAL_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickPixel::getIndex()
	Gets the colormap index of the pixel wand 
*/
PHP_METHOD(imagickpixel, getindex)
{
	php_imagickpixel_object *internp;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	
	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETVAL_LONG(PixelGetIndex(internp->pixel_wand));
}
/* }}} */

/* {{{ proto bool ImagickPixel::setIndex(int pixel_packet)
	Sets the colormap index of the pixel wand 
*/
PHP_METHOD(imagickpixel, setindex)
{
	php_imagickpixel_object *internp;
	long index;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
		return;
	}
	
	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	PixelSetIndex(internp->pixel_wand, index);
	RETURN_TRUE;
}
/* }}} */
#endif

/* {{{ proto ImagickPixel ImagickPixel::__construct([string color] )
   The ImagickPixel constructor
*/
PHP_METHOD(imagickpixel, __construct)
{
	php_imagickpixel_object *internp;
	char *color_name = NULL;
	int color_name_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &color_name, &color_name_len) == FAILURE) {
		return;
	}

	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	internp->pixel_wand = NewPixelWand();

	if (!internp->pixel_wand) {
		php_imagick_throw_exception (IMAGICKPIXEL_CLASS, "Failed to allocate PixelWand structure" TSRMLS_CC);
		return;
	}

	/* If color was given as parameter, set it here.*/
	if (color_name && color_name_len) {
		if (PixelSetColor(internp->pixel_wand, color_name) == MagickFalse) {
			php_imagick_throw_exception (IMAGICKPIXEL_CLASS, "Unable to construct ImagickPixel" TSRMLS_CC);
			return;
		}
	}
}
/* }}} */

/* {{{ proto bool ImagickPixel::setColor(string color)
	Sets the color of the pixel wand with a string (e.g. "blue", "#0000ff", "rgb(0,0,255)", "cmyk(100,100,100,10)", etc.).
*/
PHP_METHOD(imagickpixel, setcolor)
{
	char *color_name;
	int color_name_len;
	
	php_imagickpixel_object *internp;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &color_name, &color_name_len) == FAILURE) {
		return;
	}
	
	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = PixelSetColor(internp->pixel_wand, color_name);

	if (status == MagickFalse) {
		php_imagick_convert_imagickpixel_exception (internp->pixel_wand, "Unable to set ImagickPixel color" TSRMLS_CC);
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickPixel::clear()
	Clears resources associated with the wand.
*/
PHP_METHOD(imagickpixel, clear)
{
	php_imagickpixel_object *internp;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	ClearPixelWand(internp->pixel_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickPixel::isSimilar(float fuzz)
	Returns true if the distance between two colors is less than the specified distance.
*/
static
void s_is_pixelwand_similar(INTERNAL_FUNCTION_PARAMETERS, zend_bool use_quantum)
{
	zval *param;
	double fuzz;
	php_imagickpixel_object *internp;
	MagickBooleanType status;
	PixelWand *color_wand;
	zend_bool allocated;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zd", &param, &fuzz) == FAILURE) {
		return;
	}

	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	color_wand = php_imagick_zval_to_pixelwand (param, IMAGICKPIXEL_CLASS, &allocated TSRMLS_CC);
	if (!color_wand)
		return;

	status = IsPixelWandSimilar(internp->pixel_wand, color_wand, (use_quantum ? (QuantumRange * fuzz) : fuzz));
	if (allocated)
		color_wand = DestroyPixelWand (color_wand);

	if (status == MagickFalse) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}


/* {{{ proto bool ImagickPixel::isSimilar(ImagickPixel pixel, float fuzz)
	Returns true if the distance between two colors is less than the specified distance.
*/
PHP_METHOD(imagickpixel, issimilar)
{
	IMAGICK_METHOD_DEPRECATED_USE_INSTEAD("ImagickPixel", "isSimilar", "ImagickPixel", "isPixelSimilar");
	s_is_pixelwand_similar (INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */

/* {{{ proto bool ImagickPixel::isPixelSimilar(ImagickPixel pixel, float fuzz)
	Returns true if the distance between two colors is less than the specified distance.
*/
PHP_METHOD(imagickpixel, ispixelsimilar)
{
	s_is_pixelwand_similar (INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
/* }}} */

/* {{{ proto float ImagickPixel::getColorValue(int color)
	Gets the normalized color of the ImagickPixel.
*/
PHP_METHOD(imagickpixel, getcolorvalue)
{
	php_imagick_color_t color_enum;
	php_imagickpixel_object *internp;
	long color;
	double color_value = 0;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &color) == FAILURE) {
		return;
	}

	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (color <= PHP_IMAGICK_COLOR_MIN || color >= PHP_IMAGICK_COLOR_MAX) {
		php_imagick_throw_exception (IMAGICKPIXEL_CLASS, "Unknown color type" TSRMLS_CC);
		return;
	}

	color_enum = color;

	switch (color_enum) {

		case PHP_IMAGICK_COLOR_BLACK:
			color_value = PixelGetBlack(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_BLUE:
			color_value = PixelGetBlue(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_CYAN:
			color_value = PixelGetCyan(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_GREEN:
			color_value = PixelGetGreen(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_RED:
			color_value = PixelGetRed(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_YELLOW:
			color_value = PixelGetYellow(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_MAGENTA:
			color_value = PixelGetMagenta(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_OPACITY:
			color_value = PixelGetOpacity(internp->pixel_wand);
		break;

		case PHP_IMAGICK_COLOR_ALPHA:
			color_value = PixelGetAlpha(internp->pixel_wand);
		break;

#if MagickLibVersion > 0x628
		case PHP_IMAGICK_COLOR_FUZZ:
			color_value = PixelGetFuzz(internp->pixel_wand);
		break;
#endif

		default:
			php_imagick_throw_exception (IMAGICKPIXEL_CLASS, "Unknown color type" TSRMLS_CC);
			return;
		break;
	}
	RETVAL_DOUBLE(color_value);
}
/* }}} */

/* {{{ proto float ImagickPixel::setColorValue(int color, float value )
	Sets the normalized color of the ImagickPixel.
*/
PHP_METHOD(imagickpixel, setcolorvalue)
{
	php_imagick_color_t color_enum;
	php_imagickpixel_object *internp;
	long color;
	double color_value;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ld", &color, &color_value) == FAILURE) {
		return;
	}

	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (color <= PHP_IMAGICK_COLOR_MIN || color >= PHP_IMAGICK_COLOR_MAX) {
		php_imagick_throw_exception (IMAGICKPIXEL_CLASS, "Unknown color type" TSRMLS_CC);
		return;
	}

	color_enum = color;

	switch (color_enum) {

		case PHP_IMAGICK_COLOR_BLACK:
			PixelSetBlack(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_BLUE:
			PixelSetBlue(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_CYAN:
			PixelSetCyan(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_GREEN:
			PixelSetGreen(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_RED:
			PixelSetRed(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_YELLOW:
			PixelSetYellow(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_MAGENTA:
			PixelSetMagenta(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_OPACITY:
			PixelSetOpacity(internp->pixel_wand, color_value);
		break;

		case PHP_IMAGICK_COLOR_ALPHA:
			PixelSetAlpha(internp->pixel_wand, color_value);
		break;

#if MagickLibVersion > 0x628
		case PHP_IMAGICK_COLOR_FUZZ:
			PixelSetFuzz(internp->pixel_wand, color_value);
		break;
#endif

		default:
			php_imagick_throw_exception (IMAGICKPIXEL_CLASS, "Unknown color type" TSRMLS_CC);
			return;
		break;
	}
	RETVAL_TRUE;
}
/* }}} */

/* {{{ proto array ImagickPixel::getColor([bool normalized])
	Returns the color of the pixel in an array
*/
PHP_METHOD(imagickpixel, getcolor)
{
	php_imagickpixel_object *internp;
	zend_bool normalized = 0;
	double red, green, blue, alpha;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &normalized) == FAILURE) {
		return;
	}

	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	array_init(return_value);

	if (normalized == 1) {

		red   = PixelGetRed(internp->pixel_wand);
		green = PixelGetGreen(internp->pixel_wand);
		blue  = PixelGetBlue(internp->pixel_wand);
		alpha = PixelGetAlpha(internp->pixel_wand);

		add_assoc_double(return_value, "r", red);
		add_assoc_double(return_value, "g", green);
		add_assoc_double(return_value, "b", blue);
		add_assoc_double(return_value, "a", alpha);

	} else {

		/* TODO: should this be quantum range instead of hardcoded 255.. */
		red   = PixelGetRed(internp->pixel_wand)   * 255;
		green = PixelGetGreen(internp->pixel_wand) * 255;
		blue  = PixelGetBlue(internp->pixel_wand)  * 255;
		alpha = PixelGetAlpha(internp->pixel_wand);

		add_assoc_long(return_value, "r", (long) (red   > 0.0 ? red   + 0.5 : red   - 0.5));
		add_assoc_long(return_value, "g", (long) (green > 0.0 ? green + 0.5 : green - 0.5));
		add_assoc_long(return_value, "b", (long) (blue  > 0.0 ? blue  + 0.5 : blue  - 0.5));
		add_assoc_long(return_value, "a", alpha);
	}

	return;
}
/* }}} */

/* {{{ proto array ImagickPixel::getColorAsString(void )
        Returns the color as a string
*/
PHP_METHOD(imagickpixel, getcolorasstring)
{
	php_imagickpixel_object *internp;
	char *color_string;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	color_string = PixelGetColorAsString(internp->pixel_wand);
	ZVAL_STRING(return_value, color_string, 1);

	IMAGICK_FREE_MAGICK_MEMORY(color_string);
	return;
}
/* }}} */

/* {{{ proto array ImagickPixel::clone(void)
        Clones the ImagickPixel
*/
PHP_METHOD(imagickpixel, clone)
{
	php_imagickpixel_object *internp;
	php_imagickpixel_object *intern_return;
	PixelWand *pixel_wand;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	IMAGICK_METHOD_DEPRECATED("ImagickPixel", "clone");

	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	pixel_wand = php_imagick_clone_pixelwand (internp->pixel_wand);
	if (!pixel_wand) {
		php_imagick_throw_exception (IMAGICKPIXEL_CLASS, "Failed to allocate" TSRMLS_CC);
		return;
	}

	object_init_ex(return_value, php_imagickpixel_sc_entry);
	intern_return = (php_imagickpixel_object *)zend_object_store_get_object(return_value TSRMLS_CC);

	php_imagick_replace_pixelwand(intern_return, pixel_wand);
	return;
}
/* }}} */

/* {{{ proto int ImagickPixel::getColorCount()
	Returns the color count associated with this color.
*/
PHP_METHOD(imagickpixel, getcolorcount)
{
	php_imagickpixel_object *internp;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	
	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	RETVAL_LONG(PixelGetColorCount(internp->pixel_wand));
}
/* }}} */

/* {{{ proto int ImagickPixel::setColorCount(int colorCount)
	Sets the color count associated with this color.
*/
PHP_METHOD(imagickpixel, setcolorcount)
{
	php_imagickpixel_object *internp;
	long color_count;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &color_count) == FAILURE) {
		return;
	}

	internp = (php_imagickpixel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    PixelSetColorCount(internp->pixel_wand, color_count);
	RETVAL_TRUE;
}
/* }}} */
