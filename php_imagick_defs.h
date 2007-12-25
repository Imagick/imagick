/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Imagick	                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 2006-2007 Mikko Koppanen, Scott MacVicar               |
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
#ifndef PHP_IMAGICK_DEFS_H /* PHP_IMAGICK_DEFS_H */
/* Prevent double inclusion */
#define PHP_IMAGICK_DEFS_H

/* Define Extension Properties */
#define PHP_IMAGICK_EXTNAME    "imagick"
#define PHP_IMAGICK_EXTVER     "2.1.0-rc2"
#define PHP_IMAGICK_EXTNUM     20100

/* Globals, needed for the ini settings */
ZEND_BEGIN_MODULE_GLOBALS(imagick)
        zend_bool locale_fix;
ZEND_END_MODULE_GLOBALS(imagick)

#ifdef ZTS
#define IMAGICK_G(v) TSRMG(imagick_globals_id, zend_imagick_globals *, v)
#else
#define IMAGICK_G(v) (imagick_globals.v)
#endif


/* Structure for Imagick object. */
typedef struct _php_imagick_object  {
	zend_object zo;
	MagickWand *magick_wand;
	long numImages;
	int next_out_of_bound;
} php_imagick_object;

/* Structure for ImagickDraw object. */
typedef struct _php_imagickdraw_object  {
	zend_object zo;
	DrawingWand *drawing_wand;
} php_imagickdraw_object;

/* Structure for ImagickPixelIterator object. */
typedef struct _php_imagickpixeliterator_object  {
	zend_object zo;
	PixelIterator *pixel_iterator;
	long instanciated_correctly;
	int iterator_type;
#if MagickLibVersion <= 0x628
	long rows;
	long iterator_position;
#endif
} php_imagickpixeliterator_object;

/* Structure for ImagickPixel object. */
typedef struct _php_imagickpixel_object  {
    zend_object zo;
    PixelWand *pixel_wand;
	int initialized_via_iterator;
} php_imagickpixel_object;

void initializeMagickConstants();
void add_assoc_string_helper( zval *retvalue, char *name, char *key, char *hashValue TSRMLS_DC );
void throwImagickPixelException( PixelWand *pixel_wand, char *fallback, long code TSRMLS_DC );
void throwImagickPixelIteratorException( PixelIterator *pixel_iterator, char *fallback, long code TSRMLS_DC );
void throwImagickDrawException( DrawingWand *drawing_wand, char *fallback, long code TSRMLS_DC );
void throwImagickException( MagickWand *magick_wand, char *fallback, long code TSRMLS_DC );
void throwExceptionWithMessage( int type, char *description, long code TSRMLS_DC );
long getImageCount( MagickWand *magick_wand TSRMLS_DC);
char *getHashValue( HashTable *hashTable TSRMLS_DC );
int count_occurences_of( char needle, char *hayStack TSRMLS_DC);
zend_bool crop_thumbnail_image( MagickWand *magick_wand, long desired_width, long desired_height TSRMLS_DC);
int readImageIntoMagickWand( php_imagick_object *intern, char *filename, int type TSRMLS_DC );
int writeImageFromFilename( php_imagick_object *intern, char *filename, zend_bool adjoin, int type TSRMLS_DC );
char *getPseudoFilename( char *pseudoString TSRMLS_DC );
int checkIfFontIsConfigured( char *font, int fontLen TSRMLS_DC );
#if MagickLibVersion <= 0x628
void count_pixeliterator_rows( php_imagickpixeliterator_object *internpix TSRMLS_DC );
#endif

/* Define some color constants */
#define IMAGICKCOLORBLACK 11
#define IMAGICKCOLORBLUE 12
#define IMAGICKCOLORCYAN 13
#define IMAGICKCOLORGREEN 14
#define IMAGICKCOLORRED 15
#define IMAGICKCOLORYELLOW 16
#define IMAGICKCOLORMAGENTA 17
#define IMAGICKCOLOROPACITY 18
#define IMAGICKCOLORALPHA 19
#define IMAGICKCOLORFUZZ 20

/* Class enum */
#define IMAGICK_CLASS 1
#define IMAGICKDRAW_CLASS 2
#define IMAGICKPIXELITERATOR_CLASS 3
#define IMAGICKPIXEL_CLASS 4

/* Free filename constants */
#define IMAGICK_DONT_FREE_FILENAME 0
#define IMAGICK_FREE_FILENAME 1

#endif /* PHP_IMAGICK_DEFS_H */
