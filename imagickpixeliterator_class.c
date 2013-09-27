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

/* {{{ proto ImagickPixelIterator ImagickPixelIterator::__construct( Imagick source )
   The ImagickPixelIterator constructor
*/
PHP_METHOD(imagickpixeliterator, __construct)
{
	zval *magick_object;
	php_imagickpixeliterator_object *internpix;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "O", &magick_object, php_imagick_sc_entry) == FAILURE ) {
		return;
	}

	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	intern = (php_imagick_object *)zend_object_store_get_object(magick_object TSRMLS_CC);
	if (php_imagick_ensure_not_empty (intern->magick_wand) == 0)
		return;

	if (!intern->magick_wand || !IsMagickWand(intern->magick_wand)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "Invalid Imagick object passed" TSRMLS_CC);
		return;
	}

	internpix->pixel_iterator = NewPixelIterator(intern->magick_wand);
	internpix->iterator_type = 1;

#if MagickLibVersion <= 0x628
	count_pixeliterator_rows( internpix TSRMLS_CC );
#endif

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "Can not allocate ImagickPixelIterator" TSRMLS_CC);
		return;
	}
	internpix->instanciated_correctly = 1;
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickPixelIterator::resetIterator()
	Resets the pixel iterator.  Use it in conjunction with PixelGetNextIteratorRow() to iterate over all the pixels in a pixel container.
*/
PHP_METHOD(imagickpixeliterator, resetiterator)
{
	php_imagickpixeliterator_object *internpix;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	PixelResetIterator(internpix->pixel_iterator);
#if MagickLibVersion <= 0x628
	internpix->iterator_position = 0;
#endif
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickPixelIterator::syncIterator()
	Syncs the pixel iterator.
*/
PHP_METHOD(imagickpixeliterator, synciterator)
{
	php_imagickpixeliterator_object *internpix;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	PixelSyncIterator(internpix->pixel_iterator);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickPixelIterator::setIteratorFirstRow()
	Sets the pixel iterator to the first pixel row.
*/
PHP_METHOD(imagickpixeliterator, setiteratorfirstrow)
{
	php_imagickpixeliterator_object *internpix;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	PixelSetFirstIteratorRow(internpix->pixel_iterator);
#if MagickLibVersion <= 0x628
	internpix->iterator_position = 0;
#endif
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickPixelIterator::setIteratorLastRow()
	Sets the pixel iterator to the last pixel row.
*/
PHP_METHOD(imagickpixeliterator, setiteratorlastrow)
{
	php_imagickpixeliterator_object *internpix;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	PixelSetLastIteratorRow(internpix->pixel_iterator);
#if MagickLibVersion <= 0x628
	internpix->iterator_position = (internpix->rows - 1);
#endif
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto ImagickPixelIterator ImagickPixelIterator::newPixelIterator( Imagick source )
	Returns a new pixel iterator.
*/
PHP_METHOD(imagickpixeliterator, newpixeliterator)
{
	zval *magick_object;
	php_imagickpixeliterator_object *internpix;
	php_imagick_object *intern;

	/* Parse parameters given to function */
	if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "O", &magick_object, php_imagick_sc_entry) == FAILURE ) {
		return;
	}

	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	intern = (php_imagick_object *)zend_object_store_get_object(magick_object TSRMLS_CC);
	if (php_imagick_ensure_not_empty (intern->magick_wand) == 0)
		return;

	if (!intern->magick_wand || !IsMagickWand(intern->magick_wand)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "Invalid Imagick object passed" TSRMLS_CC);
		return;
	}

	internpix->pixel_iterator = NewPixelIterator(intern->magick_wand);
	internpix->iterator_type = 1;
#if MagickLibVersion <= 0x628
	count_pixeliterator_rows(internpix TSRMLS_CC);
#endif
	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "Can not allocate ImagickPixelIterator" TSRMLS_CC);
		return;
	}

	internpix->instanciated_correctly = 1;
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickPixelIterator::newPixelRegionIterator(Imagick source, int x, int y, int columns, int rows)
	Returns a new pixel iterator.
*/
PHP_METHOD(imagickpixeliterator, newpixelregioniterator)
{
	zval *magick_object;
	php_imagickpixeliterator_object *internpix;
	php_imagick_object *intern;
	long x, y, columns, rows;

	/* Parse parameters given to function */
	if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "Ollll", &magick_object, php_imagick_sc_entry, &x, &y, &columns, &rows) == FAILURE ) {
		return;
	}

	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	intern = (php_imagick_object *)zend_object_store_get_object(magick_object TSRMLS_CC);

	if (!intern->magick_wand || !IsMagickWand(intern->magick_wand)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "Invalid Imagick object passed" TSRMLS_CC);
		return;
	}

	if (php_imagick_ensure_not_empty (intern->magick_wand) == 0)
		return;

	internpix->pixel_iterator = NewPixelRegionIterator( intern->magick_wand, x, y, columns, rows);
	internpix->iterator_type = 2;
#if MagickLibVersion <= 0x628
	count_pixeliterator_rows(internpix TSRMLS_CC);
#endif
	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "Can not allocate ImagickPixelIterator" TSRMLS_CC);
		return;
	}

	internpix->instanciated_correctly = 1;
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto int ImagickPixelIterator::getIteratorRow()
	Returns the crurent pixel iterator row.
*/
PHP_METHOD(imagickpixeliterator, getiteratorrow)
{
	php_imagickpixeliterator_object *internpix;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}
#if MagickLibVersion <= 0x628
	ZVAL_LONG(return_value, internpix->iterator_position);
#else
	ZVAL_LONG(return_value, (long) PixelGetIteratorRow(internpix->pixel_iterator));
#endif
	return;
}
/* }}} */

/* {{{ proto bool ImagickPixelIterator::setIteratorRow(int row)
	Set the pixel iterator row.
*/
PHP_METHOD(imagickpixeliterator, setiteratorrow)
{
	php_imagickpixeliterator_object *internpix;
	MagickBooleanType status;
	long row;

	/* Parse parameters given to function */
	if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &row) == FAILURE ) {
		return;
	}
	
	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	status = PixelSetIteratorRow(internpix->pixel_iterator, row);

	if (status == MagickFalse) {
		php_imagick_convert_imagickpixeliterator_exception(internpix->pixel_iterator, "Unable to set iterator row" TSRMLS_CC);
		return;
	}
#if MagickLibVersion <= 0x628
	internpix->iterator_position = row;
#endif
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto array ImagickPixelIterator::getPreviousIteratorRow()
	Returns the previous row as an array of pixel wands from the pixel iterator.
*/
PHP_METHOD(imagickpixeliterator, getpreviousiteratorrow)
{
	php_imagickpixeliterator_object *internpix;
	php_imagickpixel_object *internp;
	PixelWand **wand_array;
	unsigned long num_wands, i;
	zval *tmp_pixelwand;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	wand_array = PixelGetPreviousIteratorRow(internpix->pixel_iterator, &num_wands);

#if MagickLibVersion <= 0x628
	if (internpix->iterator_position != 0) {
		internpix->iterator_position--;
	}
#endif

	if (!wand_array) {
		RETURN_NULL();
	}

	array_init(return_value);

	for (i = 0; i < num_wands; i++) {

		if (wand_array[i] != NULL && IsPixelWand (wand_array[i])) {
			MAKE_STD_ZVAL(tmp_pixelwand);
			object_init_ex(tmp_pixelwand, php_imagickpixel_sc_entry);
			internp = (php_imagickpixel_object *)zend_object_store_get_object(tmp_pixelwand TSRMLS_CC);
			php_imagick_replace_pixelwand(internp, wand_array[i]);
			internp->initialized_via_iterator = 1;
			add_next_index_zval(return_value, tmp_pixelwand);
		}
	}

	return;
}
/* }}} */


/* {{{ proto array ImagickPixelIterator::getCurrentIteratorRow()
	Returns the current row as an array of pixel wands from the pixel iterator.
*/
PHP_METHOD(imagickpixeliterator, getcurrentiteratorrow)
{
	php_imagickpixeliterator_object *internpix;
	php_imagickpixel_object *internp;
	PixelWand **wand_array;
	unsigned long num_wands;
	int i;
	zval *tmp_pixelwand;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}
#if MagickLibVersion <= 0x628
	if ( internpix->iterator_position >= internpix->rows) {
		RETURN_NULL();
	} else if ( internpix->iterator_position != 0) {
		long tmp;
		(void)PixelGetPreviousIteratorRow(internpix->pixel_iterator, &tmp);
	}
	wand_array = (PixelWand **)PixelGetNextIteratorRow(internpix->pixel_iterator, &num_wands);
#else
	wand_array = (PixelWand **)PixelGetCurrentIteratorRow(internpix->pixel_iterator, &num_wands);
#endif

	if (wand_array == NULL) {
		RETURN_NULL();
	}

	array_init(return_value);

	for (i = 0; i < num_wands; i++ ) {

		if (wand_array[i] != NULL && IsPixelWand (wand_array[i])) {
			MAKE_STD_ZVAL(tmp_pixelwand);
			object_init_ex(tmp_pixelwand, php_imagickpixel_sc_entry);
			internp = (php_imagickpixel_object *)zend_object_store_get_object(tmp_pixelwand TSRMLS_CC);
			php_imagick_replace_pixelwand(internp, wand_array[i]);
			internp->initialized_via_iterator = 1;
			add_next_index_zval(return_value, tmp_pixelwand);
		}
	}

	return;
}
/* }}} */

/* {{{ proto array ImagickPixelIterator::getNextIteratorRow()
	Returns the next row as an array of pixel wands from the pixel iterator.
*/
PHP_METHOD(imagickpixeliterator, getnextiteratorrow)
{
	php_imagickpixeliterator_object *internpix;
	php_imagickpixel_object *internp;
	PixelWand **wand_array;
	unsigned long num_wands, i;
	zval *tmp_pixelwand;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	wand_array = PixelGetNextIteratorRow(internpix->pixel_iterator, &num_wands);

#if MagickLibVersion <= 0x628
	internpix->iterator_position++;
#endif

	if (wand_array == NULL) {
		RETURN_NULL();
	}

	array_init(return_value);

	for (i = 0; i < num_wands; i++) {

		if (wand_array[i] != NULL && IsPixelWand (wand_array[i])) {

			MAKE_STD_ZVAL( tmp_pixelwand );
			object_init_ex(tmp_pixelwand, php_imagickpixel_sc_entry);
			internp = (php_imagickpixel_object *)zend_object_store_get_object(tmp_pixelwand TSRMLS_CC);
			php_imagick_replace_pixelwand(internp, wand_array[i]);
			internp->initialized_via_iterator = 1;
			add_next_index_zval(return_value, tmp_pixelwand);
		}
	}
	return;
}
/* }}} */

/* {{{ proto bool ImagickPixelIterator::destroy()
	Deallocates resources associated with a PixelIterator.
*/
PHP_METHOD(imagickpixeliterator, destroy)
{
	zval *object;
	php_imagickpixeliterator_object *internpix;
	object = getThis();

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(object TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

#if MagickLibVersion >= 0x628
	ClearPixelIterator(internpix->pixel_iterator);
#endif
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ImagickPixelIterator::clear()
	Clear resources associated with a PixelIterator.
*/
PHP_METHOD(imagickpixeliterator, clear)
{
	php_imagickpixeliterator_object *internpix;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}
/* Assertion failure with earlier versions */
#if MagickLibVersion <= 0x628
	RETURN_TRUE;
#else
	ClearPixelIterator( internpix->pixel_iterator );
	RETURN_TRUE;
#endif
}
/* }}} */

/* {{{ proto bool ImagickPixelIterator::clear()
	Clear resources associated with a PixelIterator.
*/
PHP_METHOD(imagickpixeliterator, valid)
{
	php_imagickpixeliterator_object *internpix;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	
	internpix = (php_imagickpixeliterator_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (internpix->instanciated_correctly < 1) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}

	if (!internpix->pixel_iterator || !IsPixelIterator(internpix->pixel_iterator)) {
		php_imagick_throw_exception (IMAGICKPIXELITERATOR_CLASS, "ImagickPixelIterator is not initialized correctly" TSRMLS_CC);
		return;
	}
#if MagickLibVersion <= 0x628
	if ( internpix->iterator_position < internpix->rows ) {
		RETURN_TRUE;
	}
#else
	/* Test if the current row is valid */
	if ( PixelSetIteratorRow(internpix->pixel_iterator, PixelGetIteratorRow(internpix->pixel_iterator))) {
		 RETURN_TRUE;
	}
#endif
	RETURN_FALSE;
}
/* }}} */

/* END OF PIXELITERATOR */
