#ifndef PHP_IMAGICK_H
/* Prevent double inclusion */
#define PHP_IMAGICK_H

/* Define Extension Properties */
#define PHP_IMAGICK_EXTNAME    "imagick"
#define PHP_IMAGICK_EXTVER     "2.0.1"
#define PHP_IMAGICK_EXTNUM     20001

/* Import configure options
   when building outside of
   the PHP source tree */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* Include PHP Standard Header */
#include "php.h"

/* Include magic wand header */
#include <wand/magick-wand.h>

/* Some extra headers */
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"
#include "ext/standard/php_string.h"
#include "ext/standard/info.h"
#include "php_ini.h"

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

#if MagickLibVersion > 0x628
/* Structure for ImagickPixelIterator object. */
typedef struct _php_imagickpixeliterator_object  {
	zend_object zo;
	PixelIterator *pixel_iterator;
	long instanciated_correctly;
	int iterator_type;
} php_imagickpixeliterator_object;
#endif

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
void calculateCropThumbnailDimensions( long *width, long *height, long *cropX, long *cropY, long cropWidth, long cropHeight, long imageWidth, long imageHeight TSRMLS_DC);
int readImageIntoMagickWand( php_imagick_object *intern, char *filename, int type TSRMLS_DC );
int writeImageFromFilename( php_imagick_object *intern, char *filename, zend_bool adjoin, int type TSRMLS_DC );
char *getPseudoFilename( char *pseudoString TSRMLS_DC );
int checkIfFontIsConfigured( char *font, int fontLen TSRMLS_DC );

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

/* Define the entry point symbol
 * Zend will use when loading this module
 */
extern zend_module_entry imagick_module_entry;
#define phpext_imagick_ptr &imagick_module_entry

#endif /* PHP_IMAGICK_H */

