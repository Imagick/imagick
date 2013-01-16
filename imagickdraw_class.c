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
/* {{{ proto bool ImagickDraw::resetvectorgraphics()
	Resets the vector graphics
*/
PHP_METHOD(imagickdraw, resetvectorgraphics)
{
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawResetVectorGraphics(internd->drawing_wand);
	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x649
/* {{{ proto bool ImagickDraw::getTextKerning()
	Gets the text kerning
*/
PHP_METHOD(imagickdraw, gettextkerning) 
{
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETURN_DOUBLE(DrawGetTextKerning(internd->drawing_wand));
}
/* }}} */

/* {{{ proto bool ImagickDraw::setTextKerning(float kerning)
	Sets the text kerning
*/
PHP_METHOD(imagickdraw, settextkerning) 
{
	php_imagickdraw_object *internd;
	double kerning;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &kerning) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawSetTextKerning(internd->drawing_wand, kerning);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::getTextInterwordSpacing()
	Gets the text interword spacing
*/
PHP_METHOD(imagickdraw, gettextinterwordspacing) 
{
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETURN_DOUBLE(DrawGetTextInterwordSpacing(internd->drawing_wand));
}
/* }}} */

/* {{{ proto bool ImagickDraw::setTextInterwordSpacing(float spacing)
	Sets the text interword spacing
*/
PHP_METHOD(imagickdraw, settextinterwordspacing) 
{
	php_imagickdraw_object *internd;
	double spacing;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &spacing) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawSetTextInterwordSpacing(internd->drawing_wand, spacing);
	RETURN_TRUE;
}
/* }}} */
#endif

#if MagickLibVersion > 0x655
/* {{{ proto bool ImagickDraw::getTextInterlineSpacing()
	Gets the text interword spacing
*/
PHP_METHOD(imagickdraw, gettextinterlinespacing)
{
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETURN_DOUBLE(DrawGetTextInterlineSpacing(internd->drawing_wand));
}
/* }}} */

/* {{{ proto bool ImagickDraw::setTextInterlineSpacing(float spacing)
	Sets the text interword spacing
*/
PHP_METHOD(imagickdraw, settextinterlinespacing)
{
	php_imagickdraw_object *internd;
	double spacing;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &spacing) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawSetTextInterlineSpacing(internd->drawing_wand, spacing);
	RETURN_TRUE;
}
/* }}} */
#endif

/* {{{ proto ImagickDraw ImagickDraw::__construct()
   The ImagickDraw constructor
*/
PHP_METHOD(imagickdraw, __construct)
{
	/* Empty constructor for possible future uses */
}
/* }}} */

/* {{{ proto bool ImagickDraw::circle(float ox, float oy, float px, float py)
	Draws a circle on the image.
*/
PHP_METHOD(imagickdraw, circle)
{
	double ox, oy, px, py;
	php_imagickdraw_object *internd;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &ox, &oy, &px, &py) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawCircle(internd->drawing_wand, ox, oy, px, py);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::rectangle(float x1, float y1, float x2, float y2)
	Draws a rectangle given two coordinates and using the current stroke, stroke width, and fill settings.
*/
PHP_METHOD(imagickdraw, rectangle)
{
	double x1, y1, x2, y2;
	php_imagickdraw_object *internd;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &x1, &y1, &x2, &y2) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawRectangle(internd->drawing_wand, x1, y1, x2, y2);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::roundRectangle(float x1, float y1, float x2, float y2, float rx, float ry)
	Draws a rounted rectangle given two coordinates, x & y corner radiuses and using the current stroke, stroke width, and fill settings.
*/
PHP_METHOD(imagickdraw, roundrectangle)
{
	double x1, y1, x2, y2, rx, ry;
	php_imagickdraw_object *internd;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddddd", &x1, &y1, &x2, &y2, &rx, &ry) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawRoundRectangle(internd->drawing_wand, x1, y1, x2, y2, rx, ry);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::ellipse(float ox, float oy, float rx, float ry, float start, float end)
	Draws an ellipse on the image.
*/
PHP_METHOD(imagickdraw, ellipse)
{
	double ox, oy, rx, ry, start, end;
	php_imagickdraw_object *internd;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddddd", &ox, &oy, &rx, &ry, &start, &end) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawEllipse(internd->drawing_wand, ox, oy, rx, ry, start, end);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::skewX(float degrees)
	Skews the current coordinate system in the horizontal direction.
*/
PHP_METHOD(imagickdraw, skewx)
{
	double degrees;
	php_imagickdraw_object *internd;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &degrees) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSkewX(internd->drawing_wand, degrees);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::skewY(float degrees)
	Skews the current coordinate system in the vertical direction.
*/
PHP_METHOD(imagickdraw, skewy)
{
	double degrees;
	php_imagickdraw_object *internd;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &degrees) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSkewY(internd->drawing_wand, degrees);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::translate(float x, float y)
	Applies a translation to the current coordinate system which moves the coordinate system origin to the specified coordinate.
*/
PHP_METHOD(imagickdraw, translate)
{
	double x, y;
	php_imagickdraw_object *internd;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawTranslate(internd->drawing_wand, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setFillColor(PixelWand fill_wand)
	Sets the fill color to be used for drawing filled objects.
*/
PHP_METHOD(imagickdraw, setfillcolor)
{
	zval *param;
	php_imagickdraw_object *internd;
	php_imagickpixel_object *internp;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_CAST_PARAMETER_TO_COLOR(param, internp, 2);
	DrawSetFillColor(internd->drawing_wand, internp->pixel_wand);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setResolution(float x, float y)
	Sets the resolution
*/
PHP_METHOD(imagickdraw, setresolution)
{
	char *density, buf[512];
	double x, y;
	php_imagickdraw_object *internd;
	DrawInfo *draw_info;
	DrawingWand *d_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	(void) snprintf(buf, 512, "%fx%f", x, y);
	density = AcquireString(buf);

	if (!density) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Failed to allocate memory", 2);
	}

	draw_info          = PeekDrawingWand(internd->drawing_wand);
	draw_info->density = density;
	
	d_wand = (DrawingWand *) DrawAllocateWand(draw_info, NULL);
	
	if (!d_wand) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Failed to allocate new DrawingWand structure", 2);
	}
	
	IMAGICKDRAW_REPLACE_DRAWINGWAND(internd, d_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setStrokeColor(PixelWand stroke_wand)
	Sets the color used for stroking object outlines.
*/
PHP_METHOD(imagickdraw, setstrokecolor)
{
	zval *param;
	php_imagickdraw_object *internd;
	php_imagickpixel_object *internp;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_CAST_PARAMETER_TO_COLOR(param, internp, 2);
	DrawSetStrokeColor(internd->drawing_wand, internp->pixel_wand);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setFillAlpha(float opacity)
	Sets the opacity to use when drawing using the fill color or fill texture. Fully opaque is 1.0.
*/
PHP_METHOD(imagickdraw, setfillalpha)
{
	php_imagickdraw_object *internd;
	double opacity;

	IMAGICK_METHOD_DEPRECATED("ImagickDraw", "setFillAlpha");

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &opacity) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetFillAlpha(internd->drawing_wand, opacity);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::getAntialias()
	Returns the antialias property associated with the wand.
*/
PHP_METHOD(imagickdraw, settextantialias)
{
	php_imagickdraw_object *internd;
	zend_bool antialias;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &antialias) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetTextAntialias(internd->drawing_wand, antialias);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setTextEncoding(string encoding)
	Specifies specifies the code set to use for text annotations. The only character encoding which may be specified at this time is "UTF-8" for representing Unicode as a sequence of bytes. Specify an empty string to set text encoding to the system's default. Successful text annotation using Unicode may require fonts designed to support Unicode.
*/
PHP_METHOD(imagickdraw, settextencoding)
{
	php_imagickdraw_object *internd;
	char *encoding;
	int encoding_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &encoding, &encoding_len) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetTextEncoding(internd->drawing_wand, encoding);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setStrokeAlpha(float opacity)
	Specifies the opacity of stroked object outlines.
*/
PHP_METHOD(imagickdraw, setstrokealpha)
{
	php_imagickdraw_object *internd;
	double opacity;

	IMAGICK_METHOD_DEPRECATED("ImagickDraw", "setStrokeAlpha");

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &opacity) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetStrokeAlpha(internd->drawing_wand, opacity);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setStrokeWidth(float stroke_width)
	Sets the width of the stroke used to draw object outlines.
*/
PHP_METHOD(imagickdraw, setstrokewidth)
{
	php_imagickdraw_object *internd;
	double width;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &width) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetStrokeWidth(internd->drawing_wand, width);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setFont(string font_name)
	Sets the fully-sepecified font to use when annotating with text.
*/
PHP_METHOD(imagickdraw, setfont)
{
	php_imagickdraw_object *internd;
	char *font, *absolute;
	int font_len, error = 0;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &font, &font_len) == FAILURE) {
		return;
	}

	/* Check that no empty string is passed */
	if (font_len == 0) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Can not set empty font", 2);
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	/* And if it wasn't */
	if (!check_configured_font(font, font_len TSRMLS_CC )) {

		if (!(absolute = expand_filepath(font, NULL TSRMLS_CC))) {
			IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Unable to set font", 2);
		}

		/* Do a safe-mode check for the font */
		IMAGICK_SAFE_MODE_CHECK(absolute, error);
		IMAGICKDRAW_CHECK_READ_OR_WRITE_ERROR(internd, absolute, error, IMAGICK_FREE_FILENAME);

		if (VCWD_ACCESS(absolute, F_OK|R_OK )) {
			zend_throw_exception_ex(php_imagickdraw_exception_class_entry, 2 TSRMLS_CC,
				"The given font is not found in the ImageMagick configuration and the file (%s) is not accessible", absolute);

			efree(absolute);
			return;
		}

		status = DrawSetFont(internd->drawing_wand, absolute);
		efree(absolute);

	} else {
		status = DrawSetFont(internd->drawing_wand, font);
	}

	/* No magick is going to happen */
	if (status == MagickFalse) {
		IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to set font", 1);
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setFontFamily(string font_family)
	Sets the font family to use when annotating with text.
*/
PHP_METHOD(imagickdraw, setfontfamily)
{
	php_imagickdraw_object *internd;
	char *font_family;
	int font_family_len;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &font_family, &font_family_len) == FAILURE) {
		return;
	}

	/* Check that no empty string is passed */
	if (font_family_len == 0) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Can not set empty font family", 2);
	}

	if (!check_configured_font(font_family, font_family_len TSRMLS_CC )) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Unable to set font family; parameter not found in the list of configured fonts", 2);
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = DrawSetFontFamily(internd->drawing_wand, font_family);

	/* No magick is going to happen */
	if (status == MagickFalse) {
		IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to set font family", 1);
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setFontSize(float pointsize)
	Sets the font pointsize to use when annotating with text.
*/
PHP_METHOD(imagickdraw, setfontsize)
{
	php_imagickdraw_object *internd;
	double font_size;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &font_size) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetFontSize(internd->drawing_wand, font_size);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setFontStyle(int style)
	Sets the font style to use when annotating with text. The AnyStyle enumeration acts as a wild-card "don't care" option.
*/
PHP_METHOD(imagickdraw, setfontstyle)
{
	php_imagickdraw_object *internd;
	long style_id = AnyStyle;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &style_id) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetFontStyle(internd->drawing_wand, style_id);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setFontWeight(int font_weight)
	Sets the font weight to use when annotating with text.
*/
PHP_METHOD(imagickdraw, setfontweight)
{
	php_imagickdraw_object *internd;
	long weight;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &weight) == FAILURE) {
		return;
	}

	if (weight >= 100 && weight <= 900) {

		internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

		DrawSetFontWeight(internd->drawing_wand, weight);
		RETURN_TRUE;

	} else {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Font weight valid range is 100-900", 2);
	}
}
/* }}} */

/* {{{ proto int ImagickDraw::getFontStretch(int fontStretch)
	Gets the font stretch to use when annotating with text
*/
PHP_METHOD(imagickdraw, getfontstretch)
{
	php_imagickdraw_object *internd;

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETVAL_LONG(DrawGetFontStretch(internd->drawing_wand));
}
/* }}} */

/* {{{ proto bool ImagickDraw::setFontStretch(int fontStretch)
	Sets the font stretch to use when annotating with text. The AnyStretch enumeration acts as a wild-card "don't care" option.
*/
PHP_METHOD(imagickdraw, setfontstretch)
{
	php_imagickdraw_object *internd;
	long stretch;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &stretch) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetFontStretch(internd->drawing_wand, stretch);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setStrokeAntialias(bool stroke_antialias)
	Controls whether stroked outlines are antialiased. Stroked outlines are antialiased by default.  When antialiasing is disabled stroked pixels are thresholded to determine if the stroke color or underlying canvas color should be used.
*/
PHP_METHOD(imagickdraw, setstrokeantialias)
{
	php_imagickdraw_object *internd;
	zend_bool antialias;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &antialias) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetStrokeAntialias(internd->drawing_wand, antialias);
	RETURN_TRUE;

}
/* }}} */

/* {{{ proto bool ImagickDraw::setTextAlignment(int alignment)
	Specifies a text alignment to be applied when annotating with text.
*/
PHP_METHOD(imagickdraw, settextalignment)
{
	php_imagickdraw_object *internd;
	long align;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &align) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetTextAlignment(internd->drawing_wand, align);
	RETURN_TRUE;

}
/* }}} */

/* {{{ proto bool ImagickDraw::setTextDecoration(int decoration)
	Specifies a decoration to be applied when annotating with text.
*/
PHP_METHOD(imagickdraw, settextdecoration)
{
	php_imagickdraw_object *internd;
	long decoration;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &decoration) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetTextDecoration(internd->drawing_wand, decoration);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setTextUnderColor(PixelWand under_wand)
	Specifies the color of a background rectangle to place under text annotations.
*/
PHP_METHOD(imagickdraw, settextundercolor)
{
	zval *param;
	php_imagickdraw_object *internd;
	php_imagickpixel_object *internp;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_CAST_PARAMETER_TO_COLOR(param, internp, 2);
	DrawSetTextUnderColor(internd->drawing_wand, internp->pixel_wand);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setViewbox(float x1, float y1, float x2, float y2 )
	Sets the overall canvas size to be recorded with the drawing vector data. Usually this will be specified using the same size as the canvas image. When the vector data is saved to SVG or MVG formats, the viewbox is use to specify the size of the canvas image that a viewer will render the vector data on.
*/
PHP_METHOD(imagickdraw, setviewbox)
{
	php_imagickdraw_object *internd;
	long x1, y1, x2, y2;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &x1, &y1, &x2, &y2) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetViewbox(internd->drawing_wand, x1, y1, x2, y2);
	RETURN_TRUE;
}

/* {{{ proto string ImagickDraw::getFont()
	Returns a string specifying the font used when annotating with text
*/
PHP_METHOD(imagickdraw, getfont)
{
	php_imagickdraw_object *internd;
	char *font;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	font = DrawGetFont(internd->drawing_wand);
	
	if (!font) {
		RETURN_FALSE;
	} else {
		ZVAL_STRING(return_value, font, 1);
		IMAGICK_FREE_MEMORY(char *, font);
		return;
	}
}
/* }}} */

/* {{{ proto string ImagickDraw::getFontFamily()
	Returns the font family to use when annotating with text. The value returned must be freed by the user when it is no longer needed.
*/
PHP_METHOD(imagickdraw, getfontfamily)
{
	php_imagickdraw_object *internd;
	char *font_family;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	font_family = DrawGetFontFamily(internd->drawing_wand);
	if (!font_family) {
		RETURN_FALSE;
	} else {
		ZVAL_STRING(return_value, font_family, 1);
		IMAGICK_FREE_MEMORY(char *, font_family);
		return;
	}
}
/* }}} */

/* {{{ proto float ImagickDraw::getFontSize()
	Returns the font pointsize used when annotating with text.
*/
PHP_METHOD(imagickdraw, getfontsize)
{
	php_imagickdraw_object *internd;
	double font_size;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	font_size = DrawGetFontSize(internd->drawing_wand);
	ZVAL_DOUBLE(return_value, font_size);
	return;
}
/* }}} */

/* {{{ proto int ImagickDraw::getFontStyle()
	Returns the font style used when annotating with text.
*/
PHP_METHOD(imagickdraw, getfontstyle)
{
	php_imagickdraw_object *internd;
	long font_style;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	font_style = DrawGetFontStyle(internd->drawing_wand);
	ZVAL_LONG(return_value, font_style);
	return;
}
/* }}} */

/* {{{ proto int ImagickDraw::getFontWeight()
	Returns the font weight used when annotating with text.
*/
PHP_METHOD(imagickdraw, getfontweight)
{
	php_imagickdraw_object *internd;
	long weight;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	weight = DrawGetFontWeight(internd->drawing_wand);
	ZVAL_LONG(return_value, weight);
	return;
}
/* }}} */

/* {{{ proto bool ImagickDraw::clear()
	Clears a DrawingWand resource of any accumulated commands, and resets the settings it contains to their defaults.
*/
PHP_METHOD(imagickdraw, clear)
{
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (!internd->drawing_wand) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "ImagickDraw object is not allocated properly", 2);
	}

	ClearDrawingWand(internd->drawing_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto int ImagickDraw::getTextDecoration()
	Returns the decoration applied when annotating with text.
*/
PHP_METHOD(imagickdraw, gettextdecoration)
{
	php_imagickdraw_object *internd;
	long decoration;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	decoration = DrawGetTextDecoration(internd->drawing_wand);
	ZVAL_LONG(return_value, decoration);
	return;
}
/* }}} */

/* {{{ proto string ImagickDraw::getTextEncoding()
	Returns a null-terminated string which specifies the code set used for text annotations. The string must be freed by the user once it is no longer required.
*/
PHP_METHOD(imagickdraw, gettextencoding)
{
	php_imagickdraw_object *internd;
	char *encoding;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	encoding = DrawGetTextEncoding(internd->drawing_wand);

	if (!encoding) {
		RETURN_FALSE;
	} else {
		ZVAL_STRING(return_value, encoding, 1);
		IMAGICK_FREE_MEMORY(char *, encoding);
		return;
	}
}
/* }}} */

/* {{{ proto bool ImagickDraw::destroy()
	Frees all resources associated with the drawing wand.
*/
PHP_METHOD(imagickdraw, destroy)
{
	zval *object;
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	object = getThis();
	internd = (php_imagickdraw_object *)zend_object_store_get_object(object TSRMLS_CC);

	if (!internd->drawing_wand) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "ImagickDraw object is not allocated properly", 2);
	}

	ClearDrawingWand(internd->drawing_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::annotation(float x, float y, string *text)
	Draws text on the image.
*/
PHP_METHOD(imagickdraw, annotation)
{
	php_imagickdraw_object *internd;
	double x, y;
	unsigned char *text;
	int text_len;
#if MagickLibVersion < 0x632
	char *font;
#endif

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dds", &x, &y, &text, &text_len) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
#if MagickLibVersion < 0x632
	font = DrawGetFont(internd->drawing_wand);

	/* Fixes PECL Bug #11328 */
	if (!font) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Font needs to be set before annotating an image", 2);
	}
#endif

	DrawAnnotation(internd->drawing_wand, x, y, text);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::arc(float sx, float sy, float ex, float ey, float sd, float ed)
	Draws an arc falling within a specified bounding rectangle on the image.
*/
PHP_METHOD(imagickdraw, arc)
{
	double sx, sy, ex, ey, sd, ed;
	php_imagickdraw_object *internd;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddddd", &sx, &sy, &ex, &ey, &sd, &ed) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawArc(internd->drawing_wand, sx, sy, ex, ey, sd, ed);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::matte(float x, float y, int paintMethod)
	Paints on the image's opacity channel in order to set effected pixels to transparent. to influence the opacity of pixels. The available paint methods are:
*/
PHP_METHOD(imagickdraw, matte)
{
	double x, y;
	php_imagickdraw_object *internd;
	long paint_method;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddl", &x, &y, &paint_method) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawMatte(internd->drawing_wand, x, y, paint_method);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::polygon(array coordinates)
	Draws a polygon using the current stroke, stroke width, and fill color or texture, using the specified array of coordinates.
*/
PHP_METHOD(imagickdraw, polygon)
{
	zval *coordinate_array;
	php_imagickdraw_object *internd;
	PointInfo *coordinates;
	int num_elements = 0;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &coordinate_array) == FAILURE) {
		return;
	}

	coordinates = get_pointinfo_array(coordinate_array, &num_elements TSRMLS_CC);

	if (!coordinates) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Unable to read coordinate array", 2);
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawPolygon(internd->drawing_wand, num_elements, coordinates);

	efree(coordinates);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::bezier(array coordinates)
	Draws a bezier curve through a set of points on the image.
*/
PHP_METHOD(imagickdraw, bezier)
{
	zval *coordinate_array;
	php_imagickdraw_object *internd;
	PointInfo *coordinates;
	int num_elements = 0;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &coordinate_array) == FAILURE) {
		return;
	}

	coordinates = get_pointinfo_array(coordinate_array, &num_elements TSRMLS_CC);

	if (!coordinates) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Unable to read coordinate array", 2);
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawBezier(internd->drawing_wand, num_elements, coordinates);

	efree(coordinates);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::point(float x, float y)
	Draws a point using the current stroke color and stroke thickness at the specified coordinates.
*/
PHP_METHOD(imagickdraw, point)
{
	php_imagickdraw_object *internd;
	double x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawPoint(internd->drawing_wand, x, y);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::line(float sx, float sy, float ex, float ey)
	Draws a line on the image using the current stroke color, stroke opacity, and stroke width.
*/
PHP_METHOD(imagickdraw, line)
{
	php_imagickdraw_object *internd;
	double sx, sy, ex, ey;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &sx, &sy, &ex, &ey) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawLine(internd->drawing_wand, sx, sy, ex, ey);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto ImagickDraw ImagickDraw::clone()
	Makes an exact copy of the specified wand.
*/
PHP_METHOD(imagickdraw, clone)
{
	php_imagickdraw_object *internd, *intern_return;
	DrawingWand *tmp_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	IMAGICK_METHOD_DEPRECATED("ImagickDraw", "clone");
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	tmp_wand = CloneDrawingWand(internd->drawing_wand);

	object_init_ex(return_value, php_imagickdraw_sc_entry);
	intern_return = (php_imagickdraw_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICKDRAW_REPLACE_DRAWINGWAND(intern_return, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto bool ImagickDraw::affine(array affine)
	Adjusts the current affine transformation matrix with the specified affine transformation matrix. Note that the current affine transform is adjusted rather than replaced.
*/
PHP_METHOD(imagickdraw, affine)
{
	php_imagickdraw_object *internd;
	zval *affine_matrix, **ppzval;
	HashTable *affine;
	char *matrix_elements[] = { "sx", "rx", "ry",
						        "sy", "tx", "ty" };
	int i;
	double value;
	AffineMatrix *pmatrix;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &affine_matrix) == FAILURE) {
		return;
	}

	/* Allocate space to build matrix */
	pmatrix = emalloc(sizeof(AffineMatrix));

	affine = Z_ARRVAL_P(affine_matrix);
	zend_hash_internal_pointer_reset_ex(affine, (HashPosition *) 0);

	for (i = 0; i < 6 ; i++) {

		if (zend_hash_find(affine, matrix_elements[i], 3, (void**)&ppzval) == FAILURE) {
			efree(pmatrix);
			IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "AffineMatrix should contain keys: sx, rx, ry, sy, tx and ty", 2);
		} else {
			
			zval tmp_zval, *tmp_pzval;

			tmp_zval = **ppzval;
			zval_copy_ctor(&tmp_zval);
			tmp_pzval = &tmp_zval;
			convert_to_double(tmp_pzval);

			value = Z_DVAL(tmp_zval);

			if (strcmp(matrix_elements[i], "sx") == 0) {
				pmatrix->sx = value;
			} else if (strcmp(matrix_elements[i], "rx") == 0) {
				pmatrix->rx = value;
			} else if (strcmp(matrix_elements[i], "ry") == 0) {
				pmatrix->ry = value;
			} else if (strcmp(matrix_elements[i], "sy") == 0) {
				pmatrix->sy = value;
			} else if (strcmp(matrix_elements[i], "tx") == 0) {
				pmatrix->tx = value;
			} else if (strcmp(matrix_elements[i], "ty") == 0) {
				pmatrix->ty = value;
			}
		}
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawAffine(internd->drawing_wand, pmatrix);
	efree(pmatrix);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::composite(int compose, float x, float y, float width, float height, MagickWand magick_wand)
	Composites an image onto the current image, using the specified composition operator, specified position, and at the specified size.
*/
PHP_METHOD(imagickdraw, composite)
{
	php_imagickdraw_object *internd;
	php_imagick_object *intern;
	zval *magick_obj;
	long compose;
	double x, y, width, height;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lddddO", &compose, &x, &y, &width, &height, &magick_obj, php_imagick_sc_entry) == FAILURE) {
		return;
	}

	intern = (php_imagick_object *) zend_object_store_get_object(magick_obj TSRMLS_CC);
	IMAGICK_CHECK_NOT_EMPTY(intern->magick_wand, 1, 1);

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	status = DrawComposite(internd->drawing_wand, compose, x, y, width, height, intern->magick_wand);

	if (status == MagickFalse) {
		IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to composite", 2);
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::color(float x, float y, int paintMethod)
	Draws color on image using the current fill color, starting at specified position, and using specified paint method. The available paint methods are:
*/
PHP_METHOD(imagickdraw, color)
{
	php_imagickdraw_object *internd;
	double x, y;
	long paint_method;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddl", &x, &y, &paint_method) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawColor(internd->drawing_wand, x, y, paint_method);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::comment(string comment)
	Adds a comment to a vector output stream.
*/
PHP_METHOD(imagickdraw, comment)
{
	php_imagickdraw_object *internd;
	char *comment;
	int comment_len;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &comment, &comment_len) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawComment(internd->drawing_wand, comment);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string ImagickDraw::getClipPath()
	Obtains the current clipping path ID. The value returned must be deallocated by the user when it is no longer needed.
*/
PHP_METHOD(imagickdraw, getclippath)
{
	php_imagickdraw_object *internd;
	char *clip_path;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	clip_path = DrawGetClipPath(internd->drawing_wand);

	if (!clip_path) {
		RETURN_FALSE;
	} else {
		ZVAL_STRING(return_value, clip_path, 1);
		IMAGICK_FREE_MEMORY(char *, clip_path);
		return;
	}
}
/* }}} */

/* {{{ proto int ImagickDraw::getClipRule()
	Returns the current polygon fill rule to be used by the clipping path.
*/
PHP_METHOD(imagickdraw, getcliprule)
{
	php_imagickdraw_object *internd;
	long clip_rule;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	clip_rule = DrawGetClipRule(internd->drawing_wand);

	RETVAL_LONG(clip_rule);
}
/* }}} */

/* {{{ proto int ImagickDraw::getClipUnits()
	Returns the interpretation of clip path units.
*/
PHP_METHOD(imagickdraw, getclipunits)
{
	php_imagickdraw_object *internd;
	long units;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	units = DrawGetClipUnits(internd->drawing_wand);

	RETVAL_LONG(units);
}
/* }}} */

/* {{{ proto ImagickPixel ImagickDraw::getFillColor()
	Returns the fill color used for drawing filled objects.
*/
PHP_METHOD(imagickdraw, getfillcolor)
{
	php_imagickpixel_object *internp;
	php_imagickdraw_object *internd;
	PixelWand *tmp_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	tmp_wand = NewPixelWand();
	DrawGetFillColor(internd->drawing_wand, tmp_wand);

	object_init_ex(return_value, php_imagickpixel_sc_entry);
	internp = (php_imagickpixel_object *) zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICKPIXEL_REPLACE_PIXELWAND(internp, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto float ImagickDraw::getFillOpacity()
	Returns the opacity used when drawing using the fill color or fill texture.  Fully opaque is 1.0.
*/
PHP_METHOD(imagickdraw, getfillopacity)
{
	php_imagickdraw_object *internd;
	double opacity;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	opacity = DrawGetFillOpacity(internd->drawing_wand);

	RETVAL_DOUBLE(opacity);
}
/* }}} */

/* {{{ proto int ImagickDraw::getFillRule(const DrawingWand *wand)
	Returns the fill rule used while drawing polygons.
*/
PHP_METHOD(imagickdraw, getfillrule)
{
	php_imagickdraw_object *internd;
	long fill_rule;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	fill_rule = DrawGetFillRule(internd->drawing_wand);

	RETVAL_LONG(fill_rule);
}
/* }}} */

/* {{{ proto int ImagickDraw::getGravity()
	Returns the text placement gravity used when annotating with text.
*/
PHP_METHOD(imagickdraw, getgravity)
{
	php_imagickdraw_object *internd;
	long gravity;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	gravity = DrawGetGravity(internd->drawing_wand);

	RETVAL_LONG(gravity);
}
/* }}} */

/* {{{ proto bool ImagickDraw::getStrokeAntialias()
	Returns the current stroke antialias setting. Stroked outlines are antialiased by default.  When antialiasing is disabled stroked pixels are thresholded to determine if the stroke color or underlying canvas color should be used.
*/
PHP_METHOD(imagickdraw, getstrokeantialias)
{
	php_imagickdraw_object *internd;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = DrawGetStrokeAntialias(internd->drawing_wand);

	if (status == MagickFalse) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
/* }}} */

/* {{{ proto ImagickPixel ImagickDraw::getStrokeColor(PixelWand stroke_color)
	Returns the color used for stroking object outlines.
*/
PHP_METHOD(imagickdraw, getstrokecolor)
{
	php_imagickpixel_object *internp;
	php_imagickdraw_object *internd;
	PixelWand *tmp_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	tmp_wand = NewPixelWand();
	DrawGetStrokeColor(internd->drawing_wand, tmp_wand);

	object_init_ex(return_value, php_imagickpixel_sc_entry);
	internp = (php_imagickpixel_object *) zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICKPIXEL_REPLACE_PIXELWAND(internp, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto array ImagickDraw::getStrokeDashArray()
	Returns an array representing the pattern of dashes and gaps used to stroke paths (see DrawSetStrokeDashArray). The array must be freed once it is no longer required by the user.
*/
PHP_METHOD(imagickdraw, getstrokedasharray)
{
	php_imagickdraw_object *internd;
	double *stroke_array;
	unsigned long num_elements, i;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	stroke_array = DrawGetStrokeDashArray(internd->drawing_wand, &num_elements);
	array_init(return_value);

	for (i = 0; i < num_elements ; i++) {
		add_next_index_double(return_value, stroke_array[i]);
	}

	IMAGICK_FREE_MEMORY(double *, stroke_array);
	return;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setStrokeDashArray(array dashArray)
	Specifies the pattern of dashes and gaps used to stroke paths. The strokeDashArray represents an array of numbers that specify the lengths of alternating dashes and gaps in pixels. If an odd number of values is provided, then the list of values is repeated to yield an even number of values. To remove an existing dash array, pass a zero number_elements argument and null dash_array. A typical strokeDashArray_ array might contain the members 5 3 2.
*/
PHP_METHOD(imagickdraw, setstrokedasharray)
{
	zval *param_array;
	double *double_array;
	long elements;
	php_imagickdraw_object *internd;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &param_array) == FAILURE) {
		return;
	}

	double_array = get_double_array_from_zval(param_array, &elements TSRMLS_CC);

	if (!double_array) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Can't read array", 2);
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetStrokeDashArray(internd->drawing_wand, elements, double_array);
	efree(double_array);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto float ImagickDraw::getStrokeDashOffset()
	Returns the offset into the dash pattern to start the dash.
*/
PHP_METHOD(imagickdraw, getstrokedashoffset)
{
	php_imagickdraw_object *internd;
	double offset;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	offset = DrawGetStrokeDashOffset(internd->drawing_wand);

	RETVAL_DOUBLE(offset);
}
/* }}} */

/* {{{ proto int ImagickDraw::getStrokeLineCap()
	Returns the shape to be used at the end of open subpaths when they are stroked. Values of LineCap are UndefinedCap, ButtCap, RoundCap, and SquareCap.
*/
PHP_METHOD(imagickdraw, getstrokelinecap)
{
	php_imagickdraw_object *internd;
	long line_cap;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	line_cap = DrawGetStrokeLineCap(internd->drawing_wand);

	RETVAL_LONG(line_cap);
}
/* }}} */

/* {{{ proto int ImagickDraw::getStrokeLineJoin()
	Returns the shape to be used at the corners of paths (or other vector shapes) when they are stroked. Values of LineJoin are UndefinedJoin, MiterJoin, RoundJoin, and BevelJoin.
*/
PHP_METHOD(imagickdraw, getstrokelinejoin)
{
	php_imagickdraw_object *internd;
	long line_join;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	line_join = DrawGetStrokeLineJoin(internd->drawing_wand);

	RETVAL_LONG(line_join);
}
/* }}} */

/* {{{ proto int ImagickDraw::getStrokeMiterLimit()
	Returns the miter limit. When two line segments meet at a sharp angle and miter joins have been specified for 'lineJoin', it is possible for the miter to extend far beyond the thickness of the line stroking the path. The miterLimit' imposes a limit on the ratio of the miter length to the 'lineWidth'.
*/
PHP_METHOD(imagickdraw, getstrokemiterlimit)
{
	php_imagickdraw_object *internd;
	unsigned long miter_limit;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	miter_limit = DrawGetStrokeMiterLimit(internd->drawing_wand);

	RETVAL_LONG(miter_limit);
}
/* }}} */

/* {{{ proto float ImagickDraw::getStrokeOpacity()
	Returns the opacity of stroked object outlines.
*/
PHP_METHOD(imagickdraw, getstrokeopacity)
{
	php_imagickdraw_object *internd;
	double opacity;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	opacity = DrawGetStrokeOpacity(internd->drawing_wand);

	RETVAL_DOUBLE(opacity);
}
/* }}} */

/* {{{ proto float ImagickDraw::getStrokeWidth()
	Returns the width of the stroke used to draw object outlines.
*/
PHP_METHOD(imagickdraw, getstrokewidth)
{
	php_imagickdraw_object *internd;
	double width;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	width = DrawGetStrokeWidth(internd->drawing_wand);

	RETVAL_DOUBLE(width);
}
/* }}} */

/* {{{ proto int ImagickDraw::getTextAlignment()
	Returns the alignment applied when annotating with text.
*/
PHP_METHOD(imagickdraw, gettextalignment)
{
	php_imagickdraw_object *internd;
	long align_type;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	align_type = DrawGetTextAlignment(internd->drawing_wand);

	RETVAL_LONG(align_type);
}
/* }}} */

/* {{{ proto bool ImagickDraw::getTextAntialias()
	Returns the current text antialias setting, which determines whether text is antialiased.  Text is antialiased by default.
*/
PHP_METHOD(imagickdraw, gettextantialias)
{
	php_imagickdraw_object *internd;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = DrawGetTextAntialias(internd->drawing_wand);

	if (status == MagickFalse) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
/* }}} */

/* {{{ proto string ImagickDraw::getVectorGraphics()
	Returns a null-terminated string which specifies the vector graphics generated by any graphics calls made since the wand was instantiated.  The string must be freed by the user once it is no longer required.
*/
PHP_METHOD(imagickdraw, getvectorgraphics)
{
	php_imagickdraw_object *internd;
	char *vector;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	vector = DrawGetVectorGraphics(internd->drawing_wand);

	ZVAL_STRING(return_value, vector, 1);
	IMAGICK_FREE_MEMORY(char *, vector);

	return;
}
/* }}} */

/* {{{ proto ImagickPixel ImagickDraw::getTextUnderColor(PixelWand under_color)
	Returns the color of a background rectangle to place under text annotations.
*/
PHP_METHOD(imagickdraw, gettextundercolor)
{
	php_imagickpixel_object *internp;
	php_imagickdraw_object *internd;
	PixelWand *tmp_wand;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	tmp_wand = NewPixelWand();
	
	if (!tmp_wand) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Failed to allocate memory", 2);
	}
	
	DrawGetTextUnderColor(internd->drawing_wand, tmp_wand);

	object_init_ex(return_value, php_imagickpixel_sc_entry);
	internp = (php_imagickpixel_object *) zend_object_store_get_object(return_value TSRMLS_CC);
	IMAGICKPIXEL_REPLACE_PIXELWAND(internp, tmp_wand);

	return;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathClose()
	Adds a path element to the current path which closes the current subpath by drawing a straight line from the current point to the current subpath's most recent starting point (usually, the most recent moveto point).
*/
PHP_METHOD(imagickdraw, pathclose)
{
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawPathClose(internd->drawing_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathCurveToAbsolute(float x1, float y1, float x2, float y2, float x, float y)
	Draws a cubic Bezier curve from the current point to (x,y) using (x1,y1) as the control point at the beginning of the curve and (x2,y2) as the control point at the end of the curve using absolute coordinates. At the end of the command, the new current point becomes the final (x,y) coordinate pair used in the polybezier.
*/
PHP_METHOD(imagickdraw, pathcurvetoabsolute)
{
	php_imagickdraw_object *internd;
	double x1, y1, x2, y2, x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddddd", &x1, &y1, &x2, &y2, &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathCurveToAbsolute(internd->drawing_wand, x1, y1, x2, y2, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathCurveToRelative(float x1, float y1, float x2, float y2, float x, float y)
	Draws a cubic Bezier curve from the current point to (x,y) using (x1,y1) as the control point at the beginning of the curve and (x2,y2) as the control point at the end of the curve using relative coordinates. At the end of the command, the new current point becomes the final (x,y) coordinate pair used in the polybezier.
*/
PHP_METHOD(imagickdraw, pathcurvetorelative)
{
	php_imagickdraw_object *internd;
	double x1, y1, x2, y2, x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddddd", &x1, &y1, &x2, &y2, &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathCurveToRelative(internd->drawing_wand, x1, y1, x2, y2, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathCurveToQuadraticBezierAbsolute(float x1, float y1, float x, float y)
	Draws a quadratic Bezier curve from the current point to (x,y) using (x1,y1) as the control point using absolute coordinates. At the end of the command, the new current point becomes the final (x,y) coordinate pair used in the polybezier.
*/
PHP_METHOD(imagickdraw, pathcurvetoquadraticbezierabsolute)
{
	php_imagickdraw_object *internd;
	double x1, y1, x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &x1, &y1, &x, &y) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathCurveToQuadraticBezierAbsolute(internd->drawing_wand, x1, y1, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathCurveToQuadraticBezierRelative(float x1, float y1, float x, float y)
	Draws a quadratic Bezier curve from the current point to (x,y) using (x1,y1) as the control point using relative coordinates. At the end of the command, the new current point becomes the final (x,y) coordinate pair used in the polybezier.
*/
PHP_METHOD(imagickdraw, pathcurvetoquadraticbezierrelative)
{
	php_imagickdraw_object *internd;
	double x1, y1, x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &x1, &y1, &x, &y) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathCurveToQuadraticBezierRelative(internd->drawing_wand, x1, y1, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathCurveToQuadraticBezierSmoothAbsolute(float x, float y)
	Draws a quadratic Bezier curve (using relative coordinates) from the current point to (x,y). The control point is assumed to be the reflection of the control point on the previous command relative to the current point. (If there is no previous command or if the previous command was not a DrawPathCurveToQuadraticBezierAbsolute, DrawPathCurveToQuadraticBezierRelative, DrawPathCurveToQuadraticBezierSmoothAbsolut or DrawPathCurveToQuadraticBezierSmoothRelative, assume the control point is coincident with the current point.). At the end of the command, the new current point becomes the final (x,y) coordinate pair used in the polybezier.
*/
PHP_METHOD(imagickdraw, pathcurvetoquadraticbeziersmoothabsolute)
{
	php_imagickdraw_object *internd;
	double x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathCurveToQuadraticBezierSmoothAbsolute(internd->drawing_wand, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathCurveToQuadraticBezierSmoothRelative(float x, float y)
	Draws a quadratic Bezier curve (using relative coordinates) from the current point to (x, y). The control point is assumed to be the reflection of the control point on the previous command relative to the current point. (If there is no previous command or if the previous command was not a DrawPathCurveToQuadraticBezierAbsolute, DrawPathCurveToQuadraticBezierRelative, DrawPathCurveToQuadraticBezierSmoothAbsolut or DrawPathCurveToQuadraticBezierSmoothRelative, assume the control point is coincident with the current point). At the end of the command, the new current point becomes the final (x, y) coordinate pair used in the polybezier.
*/
PHP_METHOD(imagickdraw, pathcurvetoquadraticbeziersmoothrelative)
{
	php_imagickdraw_object *internd;
	double x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathCurveToQuadraticBezierSmoothRelative(internd->drawing_wand, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathCurveToSmoothAbsolute(float x2, float y2, float x, float y)
	Draws a cubic Bezier curve from the current point to (x,y) using absolute coordinates. The first control point is assumed to be the reflection of the second control point on the previous command relative to the current point. (If there is no previous command or if the previous command was not an DrawPathCurveToAbsolute, DrawPathCurveToRelative, DrawPathCurveToSmoothAbsolute or DrawPathCurveToSmoothRelative, assume the first control point is coincident with the current point.) (x2,y2) is the second control point (i.e., the control point at the end of the curve). At the end of the command, the new current point becomes the final (x,y) coordinate pair used in the polybezier.
*/
PHP_METHOD(imagickdraw, pathcurvetosmoothabsolute)
{
	php_imagickdraw_object *internd;
	double x1, y1, x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &x1, &y1, &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathCurveToSmoothAbsolute(internd->drawing_wand, x1, y1, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathCurveToSmoothRelative(float x2, float y2, float x, float y)
	Draws a cubic Bezier curve from the current point to (x,y) using relative coordinates. The first control point is assumed to be the reflection of the second control point on the previous command relative to the current point. (If there is no previous command or if the previous command was not an DrawPathCurveToAbsolute, DrawPathCurveToRelative, DrawPathCurveToSmoothAbsolute or DrawPathCurveToSmoothRelative, assume the first control point is coincident with the current point.) (x2,y2) is the second control point (i.e., the control point at the end of the curve). At the end of the command, the new current point becomes the final (x,y) coordinate pair used in the polybezier.
*/
PHP_METHOD(imagickdraw, pathcurvetosmoothrelative)
{
	php_imagickdraw_object *internd;
	double x1, y1, x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &x1, &y1, &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathCurveToSmoothRelative(internd->drawing_wand, x1, y1, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathEllipticArcAbsolute(float rx, float ry, float x_axis_rotation, bool large_arc_flag, bool sweep_flag, float x, float y)
	Draws an elliptical arc from the current point to (x, y) using absolute coordinates. The size and orientation of the ellipse are defined by two radii (rx, ry) and an xAxisRotation, which indicates how the ellipse as a whole is rotated relative to the current coordinate system. The center (cx, cy) of the ellipse is calculated automatically to satisfy the constraints imposed by the other parameters. largeArcFlag and sweepFlag contribute to the automatic calculations and help determine how the arc is drawn. If largeArcFlag is true then draw the larger of the available arcs. If sweepFlag is true, then draw the arc matching a clock-wise rotation.
*/
PHP_METHOD(imagickdraw, pathellipticarcabsolute)
{
	php_imagickdraw_object *internd;
	double rx, ry, x_axis_rotation, x, y;
	zend_bool large_arc, sweep;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddbbdd", &rx, &ry, &x_axis_rotation, &large_arc, &sweep, &x, &y) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawPathEllipticArcAbsolute(internd->drawing_wand, rx, ry, x_axis_rotation, large_arc, sweep, x, y);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathEllipticArcRelative(float rx, float ry, float x_axis_rotation, bool large_arc_flag, bool sweep_flag, float x, float y)
	Draws an elliptical arc from the current point to (x, y) using relative coordinates. The size and orientation of the ellipse are defined by two radii (rx, ry) and an xAxisRotation, which indicates how the ellipse as a whole is rotated relative to the current coordinate system. The center (cx, cy) of the ellipse is calculated automatically to satisfy the constraints imposed by the other parameters. largeArcFlag and sweepFlag contribute to the automatic calculations and help determine how the arc is drawn. If largeArcFlag is true then draw the larger of the available arcs. If sweepFlag is true, then draw the arc matching a clock-wise rotation.
*/
PHP_METHOD(imagickdraw, pathellipticarcrelative)
{
	php_imagickdraw_object *internd;
	double rx, ry, x_axis_rotation, x, y;
	zend_bool large_arc, sweep;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddbbdd", &rx, &ry, &x_axis_rotation, &large_arc, &sweep, &x, &y) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawPathEllipticArcRelative(internd->drawing_wand, rx, ry, x_axis_rotation, large_arc, sweep, x, y);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathFinish()
	Terminates the current path.
*/
PHP_METHOD(imagickdraw, pathfinish)
{
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawPathFinish(internd->drawing_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathLineToAbsolute(float x, float y)
	Draws a line path from the current point to the given coordinate using absolute coordinates. The coordinate then becomes the new current point.
*/
PHP_METHOD(imagickdraw, pathlinetoabsolute)
{
	php_imagickdraw_object *internd;
	double x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathLineToAbsolute(internd->drawing_wand, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathLineToRelative(float x, float y)
	Draws a line path from the current point to the given coordinate using relative coordinates. The coordinate then becomes the new current point.
*/
PHP_METHOD(imagickdraw, pathlinetorelative)
{
	php_imagickdraw_object *internd;
	double x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathLineToRelative(internd->drawing_wand, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathLineToHorizontalAbsolute(float x)
	Draws a horizontal line path from the current point to the target point using absolute coordinates.  The target point then becomes the new current point.
*/
PHP_METHOD(imagickdraw, pathlinetohorizontalabsolute)
{
	php_imagickdraw_object *internd;
	double y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",  &y) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathLineToHorizontalAbsolute(internd->drawing_wand, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathLineToHorizontalRelative(float x)
	Draws a horizontal line path from the current point to the target point using relative coordinates.  The target point then becomes the new current point.
*/
PHP_METHOD(imagickdraw, pathlinetohorizontalrelative)
{
	php_imagickdraw_object *internd;
	double x;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &x) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathLineToHorizontalRelative(internd->drawing_wand, x);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathLineToVerticalAbsolute(float y)
	Draws a vertical line path from the current point to the target point using absolute coordinates.  The target point then becomes the new current point.
*/
PHP_METHOD(imagickdraw, pathlinetoverticalabsolute)
{
	php_imagickdraw_object *internd;
	double y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathLineToVerticalAbsolute(internd->drawing_wand, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathLineToVerticalRelative(float y)
	Draws a vertical line path from the current point to the target point using relative coordinates.  The target point then becomes the new current point.
*/
PHP_METHOD(imagickdraw, pathlinetoverticalrelative)
{
	php_imagickdraw_object *internd;
	double y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &y) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathLineToVerticalRelative(internd->drawing_wand, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathMoveToAbsolute(float x, float y)
	Starts a new sub-path at the given coordinate using absolute coordinates. The current point then becomes the specified coordinate.
*/
PHP_METHOD(imagickdraw, pathmovetoabsolute)
{
	php_imagickdraw_object *internd;
	double x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathMoveToAbsolute(internd->drawing_wand, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathMoveToRelative(float x, float y)
	Starts a new sub-path at the given coordinate using relative coordinates. The current point then becomes the specified coordinate.
*/
PHP_METHOD(imagickdraw, pathmovetorelative)
{
	php_imagickdraw_object *internd;
	double x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPathMoveToRelative(internd->drawing_wand, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pathStart()
	Declares the start of a path drawing list which is terminated by a matching DrawPathFinish() command. All other DrawPath commands must be enclosed between a and a DrawPathFinish() command. This is because path drawing commands are subordinate commands and they do not function by themselves.
*/
PHP_METHOD(imagickdraw, pathstart)
{
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawPathStart(internd->drawing_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::polyline(array coordinates)
	Draws a polyline using the current stroke, stroke width, and fill color or texture, using the specified array of coordinates.
*/
PHP_METHOD(imagickdraw, polyline)
{
	zval *coordinate_array;
	php_imagickdraw_object *internd;
	PointInfo *coordinates;
	int num_elements = 0;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &coordinate_array) == FAILURE) {
		return;
	}

	coordinates = get_pointinfo_array(coordinate_array, &num_elements TSRMLS_CC);

	if (!coordinates) {
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICKDRAW_CLASS, "Unable to read coordinate array", 2);
	}

	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawPolyline(internd->drawing_wand, num_elements, coordinates);

	efree(coordinates);
	RETURN_TRUE;

}
/* }}} */

/* {{{ proto bool ImagickDraw::popClipPath()
	Terminates a clip path definition.
*/
PHP_METHOD(imagickdraw, popclippath)
{
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawPopClipPath(internd->drawing_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::popDefs()
	Terminates a definition list
*/
PHP_METHOD(imagickdraw, popdefs)
{
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawPopDefs(internd->drawing_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::popPattern()
	Terminates a pattern definition.
*/
PHP_METHOD(imagickdraw, poppattern)
{
	php_imagickdraw_object *internd;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = DrawPopPattern(internd->drawing_wand);

	if (status == MagickFalse) {
		IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to terminate the pattern definition", 2);
	} else {
		RETURN_TRUE;
	}
}
/* }}} */

/* {{{ proto bool ImagickDraw::pushClipPath(string clip_mask_id)
	Starts a clip path definition which is comprized of any number of drawing commands and terminated by a DrawPopClipPath() command.
*/
PHP_METHOD(imagickdraw, pushclippath)
{
	php_imagickdraw_object *internd;
	char *clip_mask;
	int clip_mask_len;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &clip_mask, &clip_mask_len) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPushClipPath(internd->drawing_wand, clip_mask);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pushDefs()
	Indicates that commands up to a terminating DrawPopDefs() command create named elements (e.g. clip-paths, textures, etc.) which may safely be processed earlier for the sake of efficiency.
*/
PHP_METHOD(imagickdraw, pushdefs)
{
	php_imagickdraw_object *internd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	DrawPushDefs(internd->drawing_wand);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::pushPattern(string pattern_id, float x, float y, float width, float height)
	Indicates that subsequent commands up to a DrawPopPattern() command comprise the definition of a named pattern. The pattern space is assigned top left corner coordinates, a width and height, and becomes its own drawing space.  Anything which can be drawn may be used in a pattern definition. Named patterns may be used as stroke or brush definitions.
*/
PHP_METHOD(imagickdraw, pushpattern)
{	
	php_imagickdraw_object *internd;
	char *pattern_id;
	int pattern_id_len;
	double x, y, width, height;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sdddd", &pattern_id, &pattern_id_len, &x, &y, &width, &height) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawPushPattern(internd->drawing_wand, pattern_id, x, y, width, height);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool Imagick::render()
	Renders all preceding drawing commands.
*/
PHP_METHOD(imagickdraw, render)
{
	php_imagickdraw_object *internd;
	MagickBooleanType status;
	char *old_locale = NULL, *buffer = NULL;
	zend_bool restore = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	IMAGICK_SET_LOCALE(old_locale, buffer, restore);
	status = DrawRender(internd->drawing_wand);
	IMAGICK_RESTORE_LOCALE(old_locale, restore);

	if (status == MagickFalse) {
		IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to render the drawing commands", 2);
	} else {
		RETURN_TRUE;
	}
}
/* }}} */

/* {{{ proto bool ImagickDraw::rotate(float degrees)
	Applies the specified rotation to the current coordinate space.
*/
PHP_METHOD(imagickdraw, rotate)
{
	php_imagickdraw_object *internd;
	double degrees;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &degrees) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawRotate(internd->drawing_wand, degrees);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::scale(float x, float y)
	Adjusts the scaling factor to apply in the horizontal and vertical directions to the current coordinate space.
*/
PHP_METHOD(imagickdraw, scale)
{
	php_imagickdraw_object *internd;
	double x, y;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawScale(internd->drawing_wand, x, y);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setClipPath(string clip_mask)
	Associates a named clipping path with the image.  Only the areas drawn on by the clipping path will be modified as long as it remains in effect.
*/
PHP_METHOD(imagickdraw, setclippath)
{
	php_imagickdraw_object *internd;
	char *clip_mask;
	int clip_mask_len;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &clip_mask, &clip_mask_len) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	status = DrawSetClipPath(internd->drawing_wand, clip_mask);

	if (status == MagickFalse) {
		IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to set clipping path", 2);
	} else {
		RETURN_TRUE;
	}
}
/* }}} */

/* {{{ proto bool ImagickDraw::setClipRule(int fill_rule)
	Set the polygon fill rule to be used by the clipping path.
*/
PHP_METHOD(imagickdraw, setcliprule)
{
	php_imagickdraw_object *internd;
	long fill_rule;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &fill_rule) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetClipRule(internd->drawing_wand, fill_rule);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setClipUnits(int clip_units)
	Sets the interpretation of clip path units.
*/
PHP_METHOD(imagickdraw, setclipunits)
{	
	php_imagickdraw_object *internd;
	long units;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &units) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetClipUnits(internd->drawing_wand, units);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setFillOpacity(float fillOpacity)
	Sets the opacity to use when drawing using the fill color or fill texture.  Fully opaque is 1.0.
*/
PHP_METHOD(imagickdraw, setfillopacity)
{
	php_imagickdraw_object *internd;
	double fillOpacity;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &fillOpacity) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetFillOpacity(internd->drawing_wand, fillOpacity);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setFillPatternURL(string fill_url)
	Sets the URL to use as a fill pattern for filling objects. Only local URLs ("#identifier") are supported at this time. These local URLs are normally created by defining a named fill pattern with DrawPushPattern/DrawPopPattern.
*/
PHP_METHOD(imagickdraw, setfillpatternurl)
{
	php_imagickdraw_object *internd;
	char *url;
	int url_len;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &url, &url_len) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	status = DrawSetFillPatternURL(internd->drawing_wand, url);

	if (status == MagickFalse) {
		IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to set fill pattern URL", 2);
	} else {
		RETURN_TRUE;
	}
}
/* }}} */

/* {{{ proto bool ImagickDraw::setFillRule(int fill_rule)
	Sets the fill rule to use while drawing polygons.
*/
PHP_METHOD(imagickdraw, setfillrule)
{
	php_imagickdraw_object *internd;
	long fill_rule;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &fill_rule) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetFillRule(internd->drawing_wand, fill_rule);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setGravity(int gravity)
	Sets the text placement gravity to use when annotating with text.
*/
PHP_METHOD(imagickdraw, setgravity)
{
	php_imagickdraw_object *internd;
	long gravity;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &gravity) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetGravity(internd->drawing_wand, gravity);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setStrokePatternURL(string stroke_url)
	Sets the pattern used for stroking object outlines.
*/
PHP_METHOD(imagickdraw, setstrokepatternurl)
{
	php_imagickdraw_object *internd;
	char *url;
	int url_len;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &url, &url_len) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	status = DrawSetStrokePatternURL(internd->drawing_wand, url);

	if (status == MagickFalse) {
		IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to set the stroke pattern URL", 2);
	} else {
		RETURN_TRUE;
	}
}
/* }}} */

/* {{{ proto bool ImagickDraw::setStrokeDashOffset(float dash_offset)
	Specifies the offset into the dash pattern to start the dash.
*/
PHP_METHOD(imagickdraw, setstrokedashoffset)
{
	php_imagickdraw_object *internd;
	double offset;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &offset) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetStrokeDashOffset(internd->drawing_wand, offset);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setStrokeLineCap(int linecap)
	Specifies the shape to be used at the end of open subpaths when they are stroked. Values of LineCap are UndefinedCap, ButtCap, RoundCap, and SquareCap.
*/
PHP_METHOD(imagickdraw, setstrokelinecap)
{
	php_imagickdraw_object *internd;
	long line_cap;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &line_cap) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetStrokeLineCap(internd->drawing_wand, line_cap);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setStrokeLineJoin(int linejoin)
	Specifies the shape to be used at the corners of paths (or other vector shapes) when they are stroked. Values of LineJoin are UndefinedJoin, MiterJoin, RoundJoin, and BevelJoin.
*/
PHP_METHOD(imagickdraw, setstrokelinejoin)
{	
	php_imagickdraw_object *internd;
	long line_join;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &line_join) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetStrokeLineJoin(internd->drawing_wand, line_join);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setStrokeMiterLimit(int miterlimit)
	Specifies the miter limit. When two line segments meet at a sharp angle and miter joins have been specified for 'lineJoin', it is possible for the miter to extend far beyond the thickness of the line stroking the path. The miterLimit' imposes a limit on the ratio of the miter length to the 'lineWidth'.
*/
PHP_METHOD(imagickdraw, setstrokemiterlimit)
{
	php_imagickdraw_object *internd;
	long miter_limit;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &miter_limit) == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetStrokeMiterLimit(internd->drawing_wand, miter_limit);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setStrokeOpacity(float stroke_opacity)
	Specifies the opacity of stroked object outlines.
*/
PHP_METHOD(imagickdraw, setstrokeopacity)
{
	php_imagickdraw_object *internd;
	double opacity;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &opacity) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

	DrawSetStrokeOpacity(internd->drawing_wand, opacity);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickDraw::setVectorGraphics(string xml)
	Sets the vector graphics associated with the specified wand.  Use this method with DrawGetVectorGraphics() as a method to persist the vector graphics state.
*/
PHP_METHOD(imagickdraw, setvectorgraphics)
{
	php_imagickdraw_object *internd;
	char *vector;
	int vector_len;
	MagickBooleanType status;

	/* Parse parameters given to function */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &vector, &vector_len) == FAILURE) {
		return;
	}

	internd = (php_imagickdraw_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	status = DrawSetVectorGraphics(internd->drawing_wand, vector);

	if (status == MagickFalse) {
		IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to set the vector graphics", 2);
	} else {
		RETURN_TRUE;
	}
}
/* }}} */

/* {{{ proto bool ImagickDraw::pop()
	Destroys the current DrawingWand in the stack, and returns to the previously pushed DrawingWand. Multiple DrawingWands may exist. It is an error to attempt to pop more DrawingWands than have been pushed, and it is proper form to pop all DrawingWands which have been pushed.
*/
PHP_METHOD(imagickdraw, pop)
{
	php_imagickdraw_object *internd;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = PopDrawingWand(internd->drawing_wand);

	if (status == MagickFalse) {
		IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to pop the current ImagickDraw object", 2);
	} else {
		RETURN_TRUE;
	}
}
/* }}} */

/* {{{ proto bool ImagickDraw::push()
	Clones the current DrawingWand to create a new DrawingWand, which is then added to the DrawingWand stack. The original drawing DrawingWand(s) may be returned to by invoking PopDrawingWand(). The DrawingWands are stored on a DrawingWand stack. For every Pop there must have already been an equivalent Push.
*/
PHP_METHOD(imagickdraw, push)
{
	php_imagickdraw_object *internd;
	MagickBooleanType status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	
	internd = (php_imagickdraw_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	status = PushDrawingWand(internd->drawing_wand);

	if (status == MagickFalse) {
		IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to push the current ImagickDraw object", 2);
	} else {
		RETURN_TRUE;
	}
}
/* }}} */

/* END OF DRAWINGWAND METHODS */
