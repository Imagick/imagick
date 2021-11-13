
#ifndef PHP_IMAGEMAGICK_VERSION_DEFS_H
# define PHP_IMAGEMAGICK_VERSION_DEFS_H

// Q) Why are the defines done in this file?
//
// A) Checking whether an ImageMagick function is available _and_ usable is
// annoyingly difficult. It's pretty common for new functions to be introduced
// in patch releases, (e.g. 6.5.4-8) rather than in minor releases (6.5.5-0).
//
// Additionally, the api for new functions often changes quite rapidly in the
// first few versions it is available in, which means we can't just check for
// the presence of the symbol
//
// Because of those two things, the sanest thing to do is to support features
// based on the minimum minor version from which they are continually
// available and stable.
//
// Any mistake in which version is required for a feature can be fixed in a
// single place, rather than having to have duplicated info elsewhere.


#if MagickLibVersion >= 0x700
  #if MagickLibVersion >= 0x702
    #define IM_HAVE_IMAGICK_SETIMAGEINTERPOLATEMETHOD 1
  #endif
#elif MagickLibVersion >= 0x631
    #define IM_HAVE_IMAGICK_SETIMAGEINTERPOLATEMETHOD 1
#endif


#if MagickLibVersion >= 0x709
	#define IM_HAVE_IMAGICK_SETIMAGEPIXELCOLOR 1
#endif


#if MagickLibVersion >= 0x709
	#define IM_HAVE_IMAGICK_SETSEED 1
#endif

#if MagickLibVersion >= 0x709
		#define IM_HAVE_IMAGICK_WAVELETDENOISEIMAGE 1
#endif

#if MagickLibVersion >= 0x709
	#define IM_HAVE_IMAGICK_MEANSHIFTIMAGE 1
#endif

#if MagickLibVersion >= 0x710
	#define IM_HAVE_IMAGICK_KMEANSIMAGE 1
#endif

#if MagickLibVersion >= 0x709
	#define IM_HAVE_IMAGICK_RANGETHRESHOLDIMAGE 1
#endif

#if MagickLibVersion >= 0x709
	#define IM_HAVE_IMAGICK_AUTOTHRESHOLDIMAGE 1
#endif

#if MagickLibVersion >= 0x710
	#define IM_HAVE_IMAGICK_BILATERALBLURIMAGE 1
#endif

#if MagickLibVersion >= 0x709
	#define IM_HAVE_IMAGICK_CLAHEIMAGE 1
#endif

#if MagickLibVersion >= 0x702
	#define IM_HAVE_IMAGICK_CHANNELFXIMAGE 1
#endif

#if MagickLibVersion >= 0x710
	#define IM_HAVE_IMAGICK_COLORTHRESHOLDIMAGE 1
#endif

#if MagickLibVersion >= 0x709
	#define IM_HAVE_IMAGICK_COMPLEXIMAGES 1
#endif

#if MagickLibVersion >= 0x702
	#define IM_HAVE_IMAGICK_INTERPOLATIVERESIZEIMAGE 1
#endif

#if MagickLibVersion >= 0x709
	#define IM_HAVE_IMAGICK_LEVELIMAGECOLORS 1
#endif

#if MagickLibVersion >= 0x709
	#define IM_HAVE_IMAGICK_LEVELIZEIMAGE 1
#endif

#if MagickLibVersion >= 0x702
	#define IM_HAVE_IMAGICK_ORDEREDDITHERIMAGE 1
#endif

#if MagickLibVersion >= 0x70B
	#define IM_HAVE_IMAGICK_WHITEBALANCEIMAGE 1
#endif

#if MagickLibVersion >= 0x702
	#define IM_HAVE_IMAGICK_ADD_NOISE_WITH_ATTENUATE 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_DELETE_OPTION 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_GET_BACKGROUND_COLOR 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_GET_IMAGE_ARTIFACTS 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_GET_IMAGE_DISTORTIONS 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_GET_IMAGE_KURTOSIS 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_GET_IMAGE_MEAN 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_GET_IMAGE_RANGE 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_GET_INTERPOLATE_METHOD 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_GET_OPTIONS 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_GET_ORIENTATION 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_GET_RESOLUTION 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_GET_TYPE 1
#endif

#if MagickLibVersion >= 0x709
	#define IM_HAVE_IMAGICK_POLYNOMIAL_IMAGE 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_SET_DEPTH 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_SET_EXTRACT 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_SET_INTERPOLATE_METHOD 1
#endif

#if MagickLibVersion >= 0x701
	#define IM_HAVE_IMAGICK_SET_ORIENTATION 1
#endif

#if MagickLibVersion >= 0x70A
	// Technically, this may be available earlier, but the behaviour around
	// default mask changed.
	#define IM_HAVE_IMAGICK_GETSETIMAGEMASK 1

// ImageMagick forget to export the header until about 7.0.11-14...
WandExport MagickBooleanType MagickSetImageMask(
	MagickWand *wand, const PixelMask type, const MagickWand *clip_mask
);

#endif

// The above is sub-optimal as it's hard to read. It'd be better to do
// something like the below:

//#define MAGICK_LIB_RANGE(NAME, IM6_VERSION, IM7_VERSION) slash
//#if MagickLibVersion >= 0x700 slash
//  #if MagickLibVersion >= IM7_VERSION slash
//      #define IM_HAVE_##NAME 1 slash
//  #endif slash
//#elif MagickLibVersion >= 0x600 slash
//  #if MagickLibVersion >= IM6_VERSION slash
//    #define IM_HAVE_##NAME 1 slash
//  #endif slash
//#endif

// MAGICK_LIB_RANGE(IMAGICK_SETIMAGEINTERPOLATEMETHOD, 0x631, 0x702)

// But this type of macro expansion is not possible.
//
// https://stackoverflow.com/questions/48431325/is-it-possible-to-define-macro-inside-macro
// "The resulting completely macro-replaced preprocessing token sequence is not
// processed as a preprocessing directive even if it resembles one,..."
//
// An alternative would be to use code generation to build the define file as
// part of the build process. However that would involve looking at m4 config files.


#if MagickLibVersion >= 0x700
// declare symbols only defined in C source and not in header
WandExport MagickBooleanType MagickSetImageMask(
	MagickWand *wand, const PixelMask type, const MagickWand *clip_mask
);
#endif

#if MagickLibVersion >= 0x710
// declare symbols only defined in C source and not in header
WandExport MagickBooleanType MagickBilateralBlurImage(MagickWand *wand,
  const double radius,const double sigma,const double intensity_sigma,
  const double spatial_sigma);
#endif

#endif /* PHP_IMAGEMAGICK_VERSION_DEFS_H */