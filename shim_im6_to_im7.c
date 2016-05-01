
/*
   +----------------------------------------------------------------------+
   | Imagick                                                              |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include "php_imagick.h"
#include "php_imagick_defs.h"
#include "php_imagick_macros.h"
#include "php_imagick_helpers.h"
#include "php_imagick_file.h"

//derprecated 

//MagickReduceNoiseImage
//MagickMedianFilterImage
//MagickRecolorImage

//Mising
//MagickSetImageBias
//MagickSetImageClipMask


#if MagickLibVersion >= 0x700


// Functions that have been replaced by channel aware versions
MagickBooleanType MagickAdaptiveBlurImageChannel(MagickWand *wand,const ChannelType channel,const double radius, const double sigma) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickAdaptiveBlurImage(wand, radius, sigma);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickAdaptiveSharpenImageChannel(MagickWand *wand, const ChannelType channel, const double radius, const double sigma) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickAdaptiveSharpenImage(wand, radius, sigma);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickAddNoiseImageChannel(MagickWand *wand,const ChannelType channel,const NoiseType noise_type) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	//TODO - what is the value meant to be?
	status = MagickAddNoiseImage(wand, noise_type, 1.0);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickAutoGammaImageChannel(MagickWand *wand, const ChannelType channel) {
  	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickAutoGammaImage(wand);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickAutoLevelImageChannel(MagickWand *wand,
  const ChannelType channel) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickAutoLevelImage(wand);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickBlurImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickBlurImage(wand, radius,sigma);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickBrightnessContrastImageChannel(MagickWand *wand,const ChannelType channel,const double brightness, const double contrast) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickBrightnessContrastImage(wand, brightness, contrast);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickClampImageChannel(MagickWand *wand, const ChannelType channel) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickClampImage(wand);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;

}

MagickBooleanType MagickClutImageChannel(MagickWand *wand, const ChannelType channel, const MagickWand *clut_wand) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}
	//TODO - need to take parameter
	status = MagickClutImage(wand, clut_wand, AverageInterpolatePixel);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickCompositeImageChannel(MagickWand *wand,
  const ChannelType channel,const MagickWand *source_wand,
  const CompositeOperator compose,const ssize_t x,const ssize_t y) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	long clip_to_self = 1;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickCompositeImage(wand, source_wand, compose, clip_to_self, x,y);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}


MagickWand *MagickCompareImageChannels(MagickWand *wand, const MagickWand *reference, const ChannelType channel, const MetricType metric, double *distortion) {

	MagickWand *tmp_wand;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	tmp_wand = MagickCompareImages(wand, reference, metric, distortion);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}
	return tmp_wand;
}


MagickBooleanType MagickContrastStretchImageChannel(MagickWand *wand,
  const ChannelType channel,const double black_point,
  const double white_point) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickContrastStretchImage(wand, black_point, white_point);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickConvolveImageChannel(MagickWand *wand,
  const ChannelType channel, const KernelInfo *kernel){

	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickConvolveImage(wand, kernel);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}


MagickBooleanType MagickEqualizeImageChannel(MagickWand *wand,
  const ChannelType channel) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickEqualizeImage(wand);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;  
}


MagickBooleanType MagickEvaluateImageChannel(MagickWand *wand,
  const ChannelType channel,const MagickEvaluateOperator op,const double value) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickEvaluateImage(wand,op,value);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

//MagickBooleanType MagickFilterImageChannel(MagickWand *wand,
//  const ChannelType channel,const KernelInfo *kernel) {
//	MagickBooleanType status;
//	ChannelType previous_channel_mask;
//
//	if (channel != UndefinedChannel) {
//		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
//	}
//
//	status = MagickFilterImage(wand, kernel);
//
//	if (channel != UndefinedChannel) {
//		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
//	}
//
//	return status; 
//}

MagickBooleanType MagickFloodfillPaintImageChannel(MagickWand *wand,
  const ChannelType channel,const PixelWand *fill,const double fuzz,
  const PixelWand *bordercolor,const ssize_t x,const ssize_t y,
  const MagickBooleanType invert) {

	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickFloodfillPaintImage(wand, fill, fuzz, bordercolor, x, y, invert);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;  
}

MagickBooleanType MagickFunctionImageChannel(MagickWand *wand,
  const ChannelType channel,const MagickFunction function,
  const size_t number_arguments,const double *arguments) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickFunctionImage(wand,function,number_arguments,arguments);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}


MagickWand *MagickFxImageChannel(MagickWand *wand, const ChannelType channel,const char *expression) {

	MagickWand *result_wand;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	result_wand = MagickFxImage(wand, expression);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return result_wand;
}


MagickBooleanType MagickGammaImageChannel(MagickWand *wand,
  const ChannelType channel,const double gamma) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickGammaImage(wand,gamma);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickGaussianBlurImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickGaussianBlurImage(wand,radius,sigma);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

size_t MagickGetImageChannelDepth(MagickWand *wand, const ChannelType channel) {
	size_t depth;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	depth = MagickGetImageDepth(wand);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return depth;
}

MagickBooleanType MagickGetImageChannelMean(MagickWand *wand, const ChannelType channel,double *mean,
  double *standard_deviation) {
  	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickGetImageMean(wand, mean, standard_deviation);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}


MagickBooleanType MagickSetImageChannelDepth(MagickWand *wand, const ChannelType channel,
  const size_t depth) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickSetImageDepth(wand, depth);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}
  
MagickBooleanType MagickGetImageChannelDistortion(MagickWand *wand,
  const MagickWand *reference,const ChannelType channel,const MetricType metric,
  double *distortion) {
  	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickGetImageDistortion(wand, reference, metric, distortion);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}
  
MagickBooleanType MagickGetImageChannelKurtosis(MagickWand *wand,const ChannelType channel,
  double *kurtosis,double *skewness) {
  	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickGetImageKurtosis(wand,kurtosis,skewness);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickGetImageChannelRange(MagickWand *wand,const ChannelType channel,double *minima,
  double *maxima) {
  	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickGetImageRange(wand, minima, maxima);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickOrderedPosterizeImageChannel(MagickWand *wand,const ChannelType channel,
  const char *threshold_map) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickOrderedDitherImage(wand, threshold_map);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

ChannelStatistics *MagickGetImageChannelStatistics(MagickWand *wand) {
	return MagickGetImageStatistics(wand);
}

MagickBooleanType MagickHaldClutImageChannel(MagickWand *wand, const ChannelType channel,const MagickWand *hald_wand) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickHaldClutImage(wand, hald_wand);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickLevelImageChannel(MagickWand *wand,
  const ChannelType channel,const double black_point,const double gamma,
  const double white_point) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickLevelImage(wand,black_point,gamma, white_point);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickMorphologyImageChannel(MagickWand *wand,
  ChannelType channel,MorphologyMethod method,const ssize_t iterations,
  KernelInfo *kernel) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickMorphologyImage(wand, method, iterations, kernel);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickMotionBlurImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma,
  const double angle) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickMotionBlurImage(wand, radius, sigma,angle);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}


MagickBooleanType MagickNegateImageChannel(MagickWand *wand,const ChannelType channel,const MagickBooleanType gray) {

	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickNegateImage(wand, gray);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickNormalizeImageChannel(MagickWand *wand,
  const ChannelType channel) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickNormalizeImage(wand);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickOpaquePaintImageChannel(MagickWand *wand,
  const ChannelType channel,const PixelWand *target,
  const PixelWand *fill,const double fuzz,const MagickBooleanType invert) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickOpaquePaintImage(wand, target, fill, fuzz, invert);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}


MagickBooleanType MagickRandomThresholdImageChannel(MagickWand *wand,
  const ChannelType channel,const double low,const double high) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickRandomThresholdImage(wand, low, high);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickRotationalBlurImageChannel(MagickWand *wand,
  const ChannelType channel,const double angle) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickRotationalBlurImage(wand, angle);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickSelectiveBlurImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma,
  const double threshold) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickSelectiveBlurImage(wand, radius, sigma, threshold);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickSeparateImageChannel(MagickWand *wand,const ChannelType channel) {
	return MagickSeparateImage(wand, channel);
}

MagickBooleanType MagickSharpenImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickSharpenImage(wand, radius, sigma);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickSigmoidalContrastImageChannel(
  MagickWand *wand,const ChannelType channel,const MagickBooleanType sharpen,
  const double alpha,const double beta) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickSigmoidalContrastImage(wand, sharpen, alpha, beta);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

MagickBooleanType MagickSolarizeImageChannel(MagickWand *wand, const ChannelType channel,const double threshold) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickSolarizeImage(wand, threshold);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}



MagickBooleanType MagickSparseColorImageChannel(MagickWand *wand, const ChannelType channel,const SparseColorMethod method, const size_t number_arguments,const double *arguments) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickSparseColorImage(wand, method, number_arguments, arguments);

   	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}



MagickBooleanType MagickStatisticImageChannel(MagickWand *wand,
  const ChannelType channel,const StatisticType type,const size_t width,
  const size_t height) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickStatisticImage(wand, type, width, height);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}

//MagickBooleanType MagickThresholdImageChannel(MagickWand *wand,
//  const ChannelType channel,const double threshold) {
//	MagickBooleanType status;
//	ChannelType previous_channel_mask;
//
//	if (channel != UndefinedChannel) {
//		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
//	}
//
//	status = MagickThresholdImage(wand, threshold);
//
//	if (channel != UndefinedChannel) {
//		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
//	}
//
//	return status;
//}


MagickBooleanType MagickUnsharpMaskImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma,
  const double amount,const double threshold) {
	MagickBooleanType status;
	ChannelType previous_channel_mask;

	if (channel != UndefinedChannel) {
		previous_channel_mask = MagickSetImageChannelMask(wand, channel);
	}

	status = MagickUnsharpMaskImage(wand, radius, sigma, amount, threshold);

	if (channel != UndefinedChannel) {
		(void) MagickSetImageChannelMask(wand, previous_channel_mask);
	}

	return status;
}




#endif