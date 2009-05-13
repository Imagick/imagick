/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Imagick	                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 2006-2009 Mikko Koppanen, Scott MacVicar               |
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

/* Globals, needed for the ini settings */
ZEND_BEGIN_MODULE_GLOBALS(imagick)
	zend_bool locale_fix;
	zend_bool progress_monitor;
ZEND_END_MODULE_GLOBALS(imagick)

#ifdef ZTS
# define IMAGICK_G(v) TSRMG(imagick_globals_id, zend_imagick_globals *, v)
#else
# define IMAGICK_G(v) (imagick_globals.v)
#endif

ZEND_EXTERN_MODULE_GLOBALS(imagick)

#ifdef HAVE_LOCALE_H
# if defined(PHP_WIN32)
#  define IMAGICK_LC_NUMERIC_LOCALE "English"
# else
#  define IMAGICK_LC_NUMERIC_LOCALE "C"
# endif
#endif

/* Class names */
#define PHP_IMAGICK_SC_NAME "Imagick"
#define PHP_IMAGICK_EXCEPTION_SC_NAME "ImagickException"

#define PHP_IMAGICKDRAW_SC_NAME "ImagickDraw"
#define PHP_IMAGICKDRAW_EXCEPTION_SC_NAME "ImagickDrawException"

#define PHP_IMAGICKPIXEL_SC_NAME "ImagickPixel"
#define PHP_IMAGICKPIXEL_EXCEPTION_SC_NAME "ImagickPixelException"

#define PHP_IMAGICKPIXELITERATOR_SC_NAME "ImagickPixelIterator"
#define PHP_IMAGICKPIXELITERATOR_EXCEPTION_SC_NAME "ImagickPixelIteratorException"

/* Structure for Imagick object. */
typedef struct _php_imagick_object  {
	zend_object zo;
	MagickWand *magick_wand;
	char *progress_monitor_name;
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


void initialize_imagick_constants();
void add_assoc_string_helper(zval *retvalue, char *name, char *key, char *hash_value TSRMLS_DC );
void deallocate_wands(MagickWand *magick, DrawingWand *draw, PixelWand *pixel TSRMLS_DC );
void *get_pointinfo_array(zval *coordinate_array, int *num_elements TSRMLS_DC);
#if MagickLibVersion <= 0x628
void count_pixeliterator_rows(php_imagickpixeliterator_object *internpix TSRMLS_DC );
#endif

int count_occurences_of(char needle, char *haystack TSRMLS_DC );
int read_image_into_magickwand(php_imagick_object *intern, int type, char *filename, int filename_len TSRMLS_DC);
int write_image_from_filename(php_imagick_object *intern, char *filename, zend_bool adjoin, int type TSRMLS_DC);
int check_configured_font(char *font, int font_len TSRMLS_DC);
int check_write_access(char *absolute TSRMLS_DC);

MagickBooleanType php_imagick_read_image_using_imagemagick(php_imagick_object *intern, int type, char *filename, int filename_len TSRMLS_DC);



zend_bool crop_thumbnail_image(MagickWand *magick_wand, long desired_width, long desired_height TSRMLS_DC);
char *get_pseudo_filename(char* pseudo_string TSRMLS_DC);
double *get_double_array_from_zval(zval *param_array, long *num_elements TSRMLS_DC);
long *get_long_array_from_zval(zval *param_array, long *num_elements TSRMLS_DC);
unsigned char *get_char_array_from_zval(zval *param_array, long *num_elements TSRMLS_DC);

MagickBooleanType php_imagick_progress_monitor(const char *text, const MagickOffsetType offset, const MagickSizeType span, void *client_data);
int php_imagick_stream_handler(php_imagick_object *intern, php_stream *stream, char *filename, int type TSRMLS_DC);
zend_bool php_imagick_validate_map(const char *map TSRMLS_DC);

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

/* Read / write constants */
#define IMAGICK_READ_WRITE_NO_ERROR 0
#define IMAGICK_READ_WRITE_SAFE_MODE_ERROR 1
#define IMAGICK_READ_WRITE_OPEN_BASEDIR_ERROR 2
#define IMAGICK_READ_WRITE_UNDERLYING_LIBRARY 3
#define IMAGICK_READ_WRITE_PERMISSION_DENIED 4
#define IMAGICK_READ_WRITE_FILENAME_TOO_LONG 5
#define IMAGICK_READ_WRITE_PATH_DOES_NOT_EXIST 6

/* Filehandle handling */
#define IMAGICK_WRITE_IMAGE_FILE 1
#define IMAGICK_WRITE_IMAGES_FILE 2
#define IMAGICK_READ_IMAGE_FILE 3
#define IMAGICK_PING_IMAGE_FILE 4

/* Class entries */
extern zend_class_entry *php_imagick_sc_entry;
extern zend_class_entry *php_imagick_exception_class_entry;
extern zend_class_entry *php_imagickdraw_sc_entry;
extern zend_class_entry *php_imagickdraw_exception_class_entry;
extern zend_class_entry *php_imagickpixel_sc_entry;
extern zend_class_entry *php_imagickpixel_exception_class_entry;
extern zend_class_entry *php_imagickpixeliterator_sc_entry;
extern zend_class_entry *php_imagickpixeliterator_exception_class_entry;

/* Forward declarations (Imagick) */

/* The conditional methods */
#if MagickLibVersion > 0x628
PHP_METHOD(imagick, pingimageblob);
PHP_METHOD(imagick, pingimagefile);
PHP_METHOD(imagick, transposeimage);
PHP_METHOD(imagick, transverseimage);
PHP_METHOD(imagick, trimimage);
PHP_METHOD(imagick, waveimage);
PHP_METHOD(imagick, vignetteimage);
PHP_METHOD(imagick, compareimagelayers);
PHP_METHOD(imagick, optimizeimagelayers);
PHP_METHOD(imagick, uniqueimagecolors);
PHP_METHOD(imagick, getimagematte);
PHP_METHOD(imagick, setimagematte);
PHP_METHOD(imagick, adaptiveresizeimage);
PHP_METHOD(imagick, sketchimage);
PHP_METHOD(imagick, shadeimage);
PHP_METHOD(imagick, getsizeoffset);
PHP_METHOD(imagick, setsizeoffset);
PHP_METHOD(imagick, adaptiveblurimage);
PHP_METHOD(imagick, contraststretchimage);
PHP_METHOD(imagick, adaptivesharpenimage);
PHP_METHOD(imagick, randomthresholdimage);
PHP_METHOD(imagick, roundcorners);
PHP_METHOD(imagick, setiteratorindex);
PHP_METHOD(imagick, getiteratorindex);
PHP_METHOD(imagick, transformimage);
#endif
#if MagickLibVersion > 0x630
PHP_METHOD(imagick, setimageopacity);
PHP_METHOD(imagick, orderedposterizeimage);
#endif
#if MagickLibVersion > 0x631
PHP_METHOD(imagick, polaroidimage);
PHP_METHOD(imagick, getimageproperty);
PHP_METHOD(imagick, setimageproperty);
PHP_METHOD(imagick, setimageinterpolatemethod);
PHP_METHOD(imagick, getimageinterpolatemethod);
PHP_METHOD(imagick, linearstretchimage);
PHP_METHOD(imagick, getimagelength);
PHP_METHOD(imagick, extentimage);
#endif
#if MagickLibVersion > 0x633
PHP_METHOD(imagick, getimageorientation);
PHP_METHOD(imagick, setimageorientation);
#endif
#if MagickLibVersion > 0x634
PHP_METHOD(imagick, paintfloodfillimage);
#endif
#if MagickLibVersion > 0x635
PHP_METHOD(imagick, clutimage);
PHP_METHOD(imagick, getimageproperties);
PHP_METHOD(imagick, getimageprofiles);
PHP_METHOD(imagick, distortimage);
PHP_METHOD(imagick, writeimagefile);
PHP_METHOD(imagick, writeimagesfile);
PHP_METHOD(imagick, resetimagepage);
PHP_METHOD(imagick, setimageclipmask);
PHP_METHOD(imagick, getimageclipmask);
PHP_METHOD(imagick, animateimages);
PHP_METHOD(imagick, recolorimage);
#endif
#if MagickLibVersion > 0x636
PHP_METHOD(imagick, setfont);
PHP_METHOD(imagick, getfont);
PHP_METHOD(imagick, setpointsize);
PHP_METHOD(imagick, getpointsize);
PHP_METHOD(imagick, mergeimagelayers);
#endif
#if MagickLibVersion > 0x637
PHP_METHOD(imagick, floodfillpaintimage);
PHP_METHOD(imagick, opaquepaintimage);
PHP_METHOD(imagick, transparentpaintimage);
PHP_METHOD(imagick, setimagealphachannel);
#endif
#if MagickLibVersion > 0x638
PHP_METHOD(imagick, liquidrescaleimage);
PHP_METHOD(imagick, decipherimage);
PHP_METHOD(imagick, encipherimage);
#endif
#if MagickLibVersion > 0x639
PHP_METHOD(imagick, setgravity);
PHP_METHOD(imagick, getgravity);
PHP_METHOD(imagick, getimagechannelrange);
PHP_METHOD(imagick, getimagealphachannel);
#endif
#if MagickLibVersion > 0x642
PHP_METHOD(imagick, getimagechanneldistortions);
#endif
#if MagickLibVersion > 0x643
PHP_METHOD(imagick, getimagegravity);
PHP_METHOD(imagick, setimagegravity);
#endif
#if MagickLibVersion > 0x645
PHP_METHOD(imagick, importimagepixels);
PHP_METHOD(imagick, deskewimage);
PHP_METHOD(imagick, segmentimage);
PHP_METHOD(imagick, sparsecolorimage);
PHP_METHOD(imagick, remapimage);
#endif
#if MagickLibVersion > 0x646
PHP_METHOD(imagick, exportimagepixels);
#endif
#if MagickLibVersion > 0x648
PHP_METHOD(imagick, getimagechannelkurtosis);
PHP_METHOD(imagick, functionimage);
#endif
PHP_METHOD(imagick, __construct);
PHP_METHOD(imagick, __tostring);
PHP_METHOD(imagick, getpixeliterator);
PHP_METHOD(imagick, getpixelregioniterator);
PHP_METHOD(imagick, readimage);
PHP_METHOD(imagick, readimages);
PHP_METHOD(imagick, pingimage);
PHP_METHOD(imagick, readimageblob);
PHP_METHOD(imagick, readimagefile);
PHP_METHOD(imagick, displayimage);
PHP_METHOD(imagick, displayimages);
PHP_METHOD(imagick, destroy);
PHP_METHOD(imagick, clear);
PHP_METHOD(imagick, clone);
PHP_METHOD(imagick, removeimage);
PHP_METHOD(imagick, writeimage);
PHP_METHOD(imagick, writeimages);
PHP_METHOD(imagick, resetiterator);
PHP_METHOD(imagick, setfirstiterator);
PHP_METHOD(imagick, setlastiterator);
PHP_METHOD(imagick, previousimage);
PHP_METHOD(imagick, nextimage);
PHP_METHOD(imagick, haspreviousimage);
PHP_METHOD(imagick, hasnextimage);
PHP_METHOD(imagick, setimageindex);
PHP_METHOD(imagick, getimageindex);
PHP_METHOD(imagick, setimageformat);
PHP_METHOD(imagick, setimagefilename);
PHP_METHOD(imagick, getimagefilename);
PHP_METHOD(imagick, getimageformat);
PHP_METHOD(imagick, getimagemimetype);
PHP_METHOD(imagick, getimagesize);
PHP_METHOD(imagick, getimagegeometry);
PHP_METHOD(imagick, getimagecolors);
PHP_METHOD(imagick, scaleimage);
PHP_METHOD(imagick, blurimage);
PHP_METHOD(imagick, thumbnailimage);
PHP_METHOD(imagick, cropthumbnailimage);
PHP_METHOD(imagick, commentimage);
PHP_METHOD(imagick, cropimage);
PHP_METHOD(imagick, labelimage);
PHP_METHOD(imagick, valid);
PHP_METHOD(imagick, current);
PHP_METHOD(imagick, drawimage);
PHP_METHOD(imagick, getimageblob);
PHP_METHOD(imagick, getimagesblob);
PHP_METHOD(imagick, setimagecompressionquality);
PHP_METHOD(imagick, getimagecompressionquality);
PHP_METHOD(imagick, annotateimage);
PHP_METHOD(imagick, compositeimage);
PHP_METHOD(imagick, modulateimage);
PHP_METHOD(imagick, montageimage);
PHP_METHOD(imagick, identifyimage);
PHP_METHOD(imagick, thresholdimage);
PHP_METHOD(imagick, adaptivethresholdimage);
PHP_METHOD(imagick, blackthresholdimage);
PHP_METHOD(imagick, whitethresholdimage);
PHP_METHOD(imagick, appendimages);
PHP_METHOD(imagick, charcoalimage);
PHP_METHOD(imagick, normalizeimage);
PHP_METHOD(imagick, oilpaintimage);
PHP_METHOD(imagick, posterizeimage);
PHP_METHOD(imagick, radialblurimage);
PHP_METHOD(imagick, raiseimage);
PHP_METHOD(imagick, resampleimage);
PHP_METHOD(imagick, resizeimage);
PHP_METHOD(imagick, rollimage);
PHP_METHOD(imagick, rotateimage);
PHP_METHOD(imagick, sampleimage);
PHP_METHOD(imagick, solarizeimage);
PHP_METHOD(imagick, shadowimage);
PHP_METHOD(imagick, setimageattribute);
PHP_METHOD(imagick, setimagebackgroundcolor);
PHP_METHOD(imagick, setimagecompose);
PHP_METHOD(imagick, setimagecompression);
PHP_METHOD(imagick, setimagedelay);
PHP_METHOD(imagick, setimagedepth);
PHP_METHOD(imagick, setimagegamma);
PHP_METHOD(imagick, setimageiterations);
PHP_METHOD(imagick, setimagemattecolor);
PHP_METHOD(imagick, setimagepage);
PHP_METHOD(imagick, setimageprogressmonitor);
PHP_METHOD(imagick, setimageresolution);
PHP_METHOD(imagick, setimagescene);
PHP_METHOD(imagick, setimagetickspersecond);
PHP_METHOD(imagick, setimagetype);
PHP_METHOD(imagick, setimageunits);
PHP_METHOD(imagick, sharpenimage);
PHP_METHOD(imagick, shaveimage);
PHP_METHOD(imagick, shearimage);
PHP_METHOD(imagick, spliceimage);
PHP_METHOD(imagick, spreadimage);
PHP_METHOD(imagick, swirlimage);
PHP_METHOD(imagick, stripimage);
PHP_METHOD(imagick, queryformats);
PHP_METHOD(imagick, queryfonts);
PHP_METHOD(imagick, queryfontmetrics);
PHP_METHOD(imagick, steganoimage);
PHP_METHOD(imagick, motionblurimage);
PHP_METHOD(imagick, mosaicimages);
PHP_METHOD(imagick, morphimages);
PHP_METHOD(imagick, minifyimage);
PHP_METHOD(imagick, addnoiseimage);
PHP_METHOD(imagick, affinetransformimage);
PHP_METHOD(imagick, averageimages);
PHP_METHOD(imagick, borderimage);
PHP_METHOD(imagick, chopimage);
PHP_METHOD(imagick, clipimage);
PHP_METHOD(imagick, clippathimage);
PHP_METHOD(imagick, coalesceimages);
PHP_METHOD(imagick, colorfloodfillimage);
PHP_METHOD(imagick, colorizeimage);
PHP_METHOD(imagick, compareimagechannels);
PHP_METHOD(imagick, compareimages);
PHP_METHOD(imagick, contrastimage);
PHP_METHOD(imagick, combineimages);
PHP_METHOD(imagick, convolveimage);
PHP_METHOD(imagick, cyclecolormapimage);
PHP_METHOD(imagick, deconstructimages);
PHP_METHOD(imagick, despeckleimage);
PHP_METHOD(imagick, edgeimage);
PHP_METHOD(imagick, embossimage);
PHP_METHOD(imagick, enhanceimage);
PHP_METHOD(imagick, equalizeimage);
PHP_METHOD(imagick, evaluateimage);
PHP_METHOD(imagick, flattenimages);
PHP_METHOD(imagick, flipimage);
PHP_METHOD(imagick, flopimage);
PHP_METHOD(imagick, frameimage);
PHP_METHOD(imagick, fximage);
PHP_METHOD(imagick, gammaimage);
PHP_METHOD(imagick, gaussianblurimage);
PHP_METHOD(imagick, getimageattribute);
PHP_METHOD(imagick, getimagebackgroundcolor);
PHP_METHOD(imagick, getimageblueprimary);
PHP_METHOD(imagick, getimagebordercolor);
PHP_METHOD(imagick, getimagechanneldepth);
PHP_METHOD(imagick, getimagechanneldistortion);
PHP_METHOD(imagick, getimagechannelextrema);
PHP_METHOD(imagick, getimagechannelmean);
PHP_METHOD(imagick, getimagechannelstatistics);
PHP_METHOD(imagick, getimagecolormapcolor);
PHP_METHOD(imagick, getimagecolorspace);
PHP_METHOD(imagick, getimagecompose);
PHP_METHOD(imagick, getimagedelay);
PHP_METHOD(imagick, getimagedepth);
PHP_METHOD(imagick, getimagedistortion);
PHP_METHOD(imagick, getimageextrema);
PHP_METHOD(imagick, getimagedispose);
PHP_METHOD(imagick, getimagegamma);
PHP_METHOD(imagick, getimagegreenprimary);
PHP_METHOD(imagick, getimageheight);
PHP_METHOD(imagick, getimagehistogram);
PHP_METHOD(imagick, getimageinterlacescheme);
PHP_METHOD(imagick, getimageiterations);
PHP_METHOD(imagick, getimagemattecolor);
PHP_METHOD(imagick, getimagepage);
PHP_METHOD(imagick, getimagepixelcolor);
PHP_METHOD(imagick, getimageprofile);
PHP_METHOD(imagick, getimageredprimary);
PHP_METHOD(imagick, getimagerenderingintent);
PHP_METHOD(imagick, getimageresolution);
PHP_METHOD(imagick, getimagescene);
PHP_METHOD(imagick, getimagesignature);
PHP_METHOD(imagick, getimagetickspersecond);
PHP_METHOD(imagick, getimagetype);
PHP_METHOD(imagick, getimageunits);
PHP_METHOD(imagick, getimagevirtualpixelmethod);
PHP_METHOD(imagick, getimagewhitepoint);
PHP_METHOD(imagick, getimagewidth);
PHP_METHOD(imagick, getnumberimages);
PHP_METHOD(imagick, getimagetotalinkdensity);
PHP_METHOD(imagick, getimageregion);
PHP_METHOD(imagick, implodeimage);
PHP_METHOD(imagick, levelimage);
PHP_METHOD(imagick, magnifyimage);
PHP_METHOD(imagick, mapimage);
PHP_METHOD(imagick, mattefloodfillimage);
PHP_METHOD(imagick, medianfilterimage);
PHP_METHOD(imagick, negateimage);
PHP_METHOD(imagick, paintopaqueimage);
PHP_METHOD(imagick, painttransparentimage);
PHP_METHOD(imagick, previewimages);
PHP_METHOD(imagick, profileimage);
PHP_METHOD(imagick, quantizeimage);
PHP_METHOD(imagick, quantizeimages);
PHP_METHOD(imagick, reducenoiseimage);
PHP_METHOD(imagick, removeimageprofile);
PHP_METHOD(imagick, separateimagechannel);
PHP_METHOD(imagick, sepiatoneimage);
PHP_METHOD(imagick, setimagebias);
PHP_METHOD(imagick, setimageblueprimary);
PHP_METHOD(imagick, setimagebordercolor);
PHP_METHOD(imagick, setimagechanneldepth);
PHP_METHOD(imagick, setimagecolormapcolor);
PHP_METHOD(imagick, setimagecolorspace);
PHP_METHOD(imagick, setimagedispose);
PHP_METHOD(imagick, setimageextent);
PHP_METHOD(imagick, setimagegreenprimary);
PHP_METHOD(imagick, setimageinterlacescheme);
PHP_METHOD(imagick, setimageprofile);
PHP_METHOD(imagick, setimageredprimary);
PHP_METHOD(imagick, setimagerenderingintent);
PHP_METHOD(imagick, setimagevirtualpixelmethod);
PHP_METHOD(imagick, setimagewhitepoint);
PHP_METHOD(imagick, sigmoidalcontrastimage);
PHP_METHOD(imagick, stereoimage);
PHP_METHOD(imagick, textureimage);
PHP_METHOD(imagick, tintimage);
PHP_METHOD(imagick, unsharpmaskimage);
PHP_METHOD(imagick, getimage);
PHP_METHOD(imagick, setimage);
PHP_METHOD(imagick, addimage);
PHP_METHOD(imagick, newimage);
PHP_METHOD(imagick, newpseudoimage);
PHP_METHOD(imagick, getcompression);
PHP_METHOD(imagick, getcompressionquality);
PHP_METHOD(imagick, getcopyright);
PHP_METHOD(imagick, getfilename);
PHP_METHOD(imagick, getformat);
PHP_METHOD(imagick, gethomeurl);
PHP_METHOD(imagick, getinterlacescheme);
PHP_METHOD(imagick, getoption);
PHP_METHOD(imagick, getpackagename);
PHP_METHOD(imagick, getpage);
PHP_METHOD(imagick, getquantumdepth);
PHP_METHOD(imagick, getquantumrange);
PHP_METHOD(imagick, getreleasedate);
PHP_METHOD(imagick, getresource);
PHP_METHOD(imagick, getresourcelimit);
PHP_METHOD(imagick, getsamplingfactors);
PHP_METHOD(imagick, getsize);
PHP_METHOD(imagick, getversion);
PHP_METHOD(imagick, setbackgroundcolor);
PHP_METHOD(imagick, setcompression);
PHP_METHOD(imagick, setcompressionquality);
PHP_METHOD(imagick, setfilename);
PHP_METHOD(imagick, setformat);
PHP_METHOD(imagick, setinterlacescheme);
PHP_METHOD(imagick, setoption);
PHP_METHOD(imagick, setpage);
PHP_METHOD(imagick, setresourcelimit);
PHP_METHOD(imagick, setresolution);
PHP_METHOD(imagick, setsamplingfactors);
PHP_METHOD(imagick, setsize);
PHP_METHOD(imagick, settype);

/* Forward declarations (ImagickDraw) */
#if MagickLibVersion > 0x628
PHP_METHOD(imagickdraw, resetvectorgraphics);
#endif
#if MagickLibVersion > 0x649
PHP_METHOD(imagickdraw, gettextkerning);
PHP_METHOD(imagickdraw, settextkerning);
PHP_METHOD(imagickdraw, gettextinterwordspacing);
PHP_METHOD(imagickdraw, settextinterwordspacing);
#endif
PHP_METHOD(imagickdraw, __construct);
PHP_METHOD(imagickdraw, setfillcolor);
PHP_METHOD(imagickdraw, setfillalpha);
PHP_METHOD(imagickdraw, setstrokecolor);
PHP_METHOD(imagickdraw, setstrokealpha);
PHP_METHOD(imagickdraw, setstrokewidth);
PHP_METHOD(imagickdraw, setfont);
PHP_METHOD(imagickdraw, setfontfamily);
PHP_METHOD(imagickdraw, setfontsize);
PHP_METHOD(imagickdraw, setfontstyle);
PHP_METHOD(imagickdraw, setfontweight);
PHP_METHOD(imagickdraw, getfont);
PHP_METHOD(imagickdraw, getfontfamily);
PHP_METHOD(imagickdraw, getfontsize);
PHP_METHOD(imagickdraw, getfontstyle);
PHP_METHOD(imagickdraw, getfontweight);
PHP_METHOD(imagickdraw, clear);
PHP_METHOD(imagickdraw, destroy);
PHP_METHOD(imagickdraw, annotation);
PHP_METHOD(imagickdraw, settextantialias);
PHP_METHOD(imagickdraw, settextencoding);
PHP_METHOD(imagickdraw, rectangle);
PHP_METHOD(imagickdraw, roundrectangle);
PHP_METHOD(imagickdraw, ellipse);
PHP_METHOD(imagickdraw, circle);
PHP_METHOD(imagickdraw, skewx);
PHP_METHOD(imagickdraw, skewy);
PHP_METHOD(imagickdraw, translate);
PHP_METHOD(imagickdraw, line);
PHP_METHOD(imagickdraw, polygon);
PHP_METHOD(imagickdraw, arc);
PHP_METHOD(imagickdraw, matte);
PHP_METHOD(imagickdraw, point);
PHP_METHOD(imagickdraw, getfontstretch);
PHP_METHOD(imagickdraw, setfontstretch);
PHP_METHOD(imagickdraw, settextalignment);
PHP_METHOD(imagickdraw, settextdecoration);
PHP_METHOD(imagickdraw, settextundercolor);
PHP_METHOD(imagickdraw, setviewbox);
PHP_METHOD(imagickdraw, gettextdecoration);
PHP_METHOD(imagickdraw, gettextencoding);
PHP_METHOD(imagickdraw, clone);
PHP_METHOD(imagickdraw, affine);
PHP_METHOD(imagickdraw, bezier);
PHP_METHOD(imagickdraw, composite);
PHP_METHOD(imagickdraw, color);
PHP_METHOD(imagickdraw, comment);
PHP_METHOD(imagickdraw, getclippath);
PHP_METHOD(imagickdraw, getcliprule);
PHP_METHOD(imagickdraw, getclipunits);
PHP_METHOD(imagickdraw, getfillcolor);
PHP_METHOD(imagickdraw, getfillopacity);
PHP_METHOD(imagickdraw, getfillrule);
PHP_METHOD(imagickdraw, getgravity);
PHP_METHOD(imagickdraw, getstrokeantialias);
PHP_METHOD(imagickdraw, getstrokecolor);
PHP_METHOD(imagickdraw, getstrokedasharray);
PHP_METHOD(imagickdraw, getstrokedashoffset);
PHP_METHOD(imagickdraw, getstrokelinecap);
PHP_METHOD(imagickdraw, getstrokelinejoin);
PHP_METHOD(imagickdraw, getstrokemiterlimit);
PHP_METHOD(imagickdraw, getstrokeopacity);
PHP_METHOD(imagickdraw, getstrokewidth);
PHP_METHOD(imagickdraw, gettextalignment);
PHP_METHOD(imagickdraw, gettextantialias);
PHP_METHOD(imagickdraw, getvectorgraphics);
PHP_METHOD(imagickdraw, gettextundercolor);
PHP_METHOD(imagickdraw, pathclose);
PHP_METHOD(imagickdraw, pathcurvetoabsolute);
PHP_METHOD(imagickdraw, pathcurvetorelative);
PHP_METHOD(imagickdraw, pathcurvetoquadraticbezierabsolute);
PHP_METHOD(imagickdraw, pathcurvetoquadraticbezierrelative);
PHP_METHOD(imagickdraw, pathcurvetoquadraticbeziersmoothabsolute);
PHP_METHOD(imagickdraw, pathcurvetoquadraticbeziersmoothrelative);
PHP_METHOD(imagickdraw, pathcurvetosmoothabsolute);
PHP_METHOD(imagickdraw, pathcurvetosmoothrelative);
PHP_METHOD(imagickdraw, pathellipticarcabsolute);
PHP_METHOD(imagickdraw, pathellipticarcrelative);
PHP_METHOD(imagickdraw, pathfinish);
PHP_METHOD(imagickdraw, pathlinetoabsolute);
PHP_METHOD(imagickdraw, pathlinetorelative);
PHP_METHOD(imagickdraw, pathlinetohorizontalabsolute);
PHP_METHOD(imagickdraw, pathlinetohorizontalrelative);
PHP_METHOD(imagickdraw, pathlinetoverticalabsolute);
PHP_METHOD(imagickdraw, pathlinetoverticalrelative);
PHP_METHOD(imagickdraw, pathmovetoabsolute);
PHP_METHOD(imagickdraw, pathmovetorelative);
PHP_METHOD(imagickdraw, pathstart);
PHP_METHOD(imagickdraw, polyline);
PHP_METHOD(imagickdraw, popclippath);
PHP_METHOD(imagickdraw, popdefs);
PHP_METHOD(imagickdraw, poppattern);
PHP_METHOD(imagickdraw, pushclippath);
PHP_METHOD(imagickdraw, pushdefs);
PHP_METHOD(imagickdraw, pushpattern);
PHP_METHOD(imagickdraw, render);
PHP_METHOD(imagickdraw, rotate);
PHP_METHOD(imagickdraw, scale);
PHP_METHOD(imagickdraw, setclippath);
PHP_METHOD(imagickdraw, setcliprule);
PHP_METHOD(imagickdraw, setclipunits);
PHP_METHOD(imagickdraw, setfillopacity);
PHP_METHOD(imagickdraw, setfillpatternurl);
PHP_METHOD(imagickdraw, setfillrule);
PHP_METHOD(imagickdraw, setgravity);
PHP_METHOD(imagickdraw, setstrokepatternurl);
PHP_METHOD(imagickdraw, setstrokeantialias);
PHP_METHOD(imagickdraw, setstrokedasharray);
PHP_METHOD(imagickdraw, setstrokedashoffset);
PHP_METHOD(imagickdraw, setstrokelinecap);
PHP_METHOD(imagickdraw, setstrokelinejoin);
PHP_METHOD(imagickdraw, setstrokemiterlimit);
PHP_METHOD(imagickdraw, setstrokeopacity);
PHP_METHOD(imagickdraw, setvectorgraphics);
PHP_METHOD(imagickdraw, pop);
PHP_METHOD(imagickdraw, push);

/* Imagick Pixel iterator */
PHP_METHOD(imagickpixeliterator, __construct);
PHP_METHOD(imagickpixeliterator, newpixeliterator);
PHP_METHOD(imagickpixeliterator, newpixelregioniterator);
PHP_METHOD(imagickpixeliterator, getiteratorrow);
PHP_METHOD(imagickpixeliterator, setiteratorrow);
PHP_METHOD(imagickpixeliterator, getpreviousiteratorrow);
PHP_METHOD(imagickpixeliterator, getcurrentiteratorrow);
PHP_METHOD(imagickpixeliterator, getnextiteratorrow);
PHP_METHOD(imagickpixeliterator, setiteratorfirstrow);
PHP_METHOD(imagickpixeliterator, setiteratorlastrow);
PHP_METHOD(imagickpixeliterator, resetiterator);
PHP_METHOD(imagickpixeliterator, synciterator);
PHP_METHOD(imagickpixeliterator, destroy);
PHP_METHOD(imagickpixeliterator, clear);
PHP_METHOD(imagickpixeliterator, valid);

/* ImagickPixel */
#if MagickLibVersion > 0x628
PHP_METHOD(imagickpixel, gethsl);
PHP_METHOD(imagickpixel, sethsl);
PHP_METHOD(imagickpixel, getcolorvaluequantum);
PHP_METHOD(imagickpixel, setcolorvaluequantum);
PHP_METHOD(imagickpixel, getindex);
PHP_METHOD(imagickpixel, setindex);
#endif
PHP_METHOD(imagickpixel, __construct);
PHP_METHOD(imagickpixel, setcolor);
PHP_METHOD(imagickpixel, clear);
PHP_METHOD(imagickpixel, destroy);
PHP_METHOD(imagickpixel, issimilar);
PHP_METHOD(imagickpixel, getcolorvalue);
PHP_METHOD(imagickpixel, setcolorvalue);
PHP_METHOD(imagickpixel, getcolor);
PHP_METHOD(imagickpixel, getcolorasstring);
PHP_METHOD(imagickpixel, getcolorcount);
PHP_METHOD(imagickpixel, setcolorcount);
PHP_METHOD(imagickpixel, clone);

#endif /* PHP_IMAGICK_DEFS_H */
