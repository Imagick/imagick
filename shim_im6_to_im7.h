

#if MagickLibVersion >= 0x700


// Functions that have been renamed
ChannelStatistics *MagickGetImageChannelStatistics(MagickWand *wand);
MagickWand *MagickCompareImageChannels(
	MagickWand *wand,
	const MagickWand *reference,
	const ChannelType channel,
	const MetricType metric,
	double *distortion
);

MagickBooleanType MagickConvolveImageChannel(MagickWand *wand,
  const ChannelType channel, const KernelInfo *kernel);

//Function that used to be channel only
MagickBooleanType MagickFloodfillPaintImageChannel(MagickWand *wand,
  const ChannelType channel,const PixelWand *fill,const double fuzz,
  const PixelWand *bordercolor,const ssize_t x,const ssize_t y,
  const MagickBooleanType invert
);

MagickBooleanType MagickSparseColorImageChannel(MagickWand *wand, const ChannelType channel,const SparseColorMethod method, const size_t number_arguments,const double *arguments);

// Functions that have been replaced by channel aware versions
MagickBooleanType MagickAdaptiveBlurImageChannel(
	MagickWand *magick_wand,
	const ChannelType channel,
	const double radius,
	const double sigma
);
MagickBooleanType MagickAdaptiveSharpenImageChannel(
	MagickWand *wand,
	const ChannelType channel,
	const double radius,
	const double sigma
);
MagickBooleanType MagickAddNoiseImageChannel(
	MagickWand *wand,
	const ChannelType channel,
	const NoiseType noise_type
);
MagickBooleanType MagickAutoGammaImageChannel(MagickWand *wand,
  const ChannelType channel);
MagickBooleanType MagickAutoLevelImageChannel(MagickWand *wand,
  const ChannelType channel);
MagickBooleanType MagickBlurImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma);
MagickBooleanType MagickBrightnessContrastImageChannel(MagickWand *wand,
  const ChannelType channel,const double brightness,
  const double contrast);
MagickBooleanType MagickClampImageChannel(MagickWand *wand,
  const ChannelType channel);
MagickBooleanType MagickClutImageChannel(MagickWand *wand,
  const ChannelType channel,const MagickWand *clut_wand);
MagickBooleanType MagickCompositeImageChannel(MagickWand *wand,
  const ChannelType channel,const MagickWand *composite_wand,
  const CompositeOperator compose,const ssize_t x,const ssize_t y);
MagickBooleanType MagickContrastStretchImageChannel(MagickWand *wand,
  const ChannelType channel,const double black_point,
  const double white_point); 
MagickBooleanType MagickEqualizeImageChannel(MagickWand *wand,
  const ChannelType channel);
MagickBooleanType MagickEvaluateImageChannel(MagickWand *wand,
  const ChannelType channel,const MagickEvaluateOperator op,
  const double value);
MagickBooleanType MagickGetImageChannelDistortion(MagickWand *wand,
  const MagickWand *reference,const ChannelType channel,const MetricType metric,
  double *distortion);
MagickBooleanType MagickGetImageChannelKurtosis(MagickWand *wand,const ChannelType channel,
  double *kurtosis,double *skewness);
MagickBooleanType MagickGetImageChannelRange(MagickWand *wand,const ChannelType channel,double *minima,
  double *maxima);
size_t MagickGetImageChannelDepth(MagickWand *wand, const ChannelType channel);
MagickBooleanType MagickSetImageChannelDepth(MagickWand *wand, const ChannelType channel,
  const size_t depth);
MagickBooleanType MagickGetImageChannelMean(MagickWand *wand, const ChannelType channel,double *mean,
  double *standard_deviation);  
//MagickBooleanType MagickFilterImageChannel(MagickWand *wand,
//  const ChannelType channel,const KernelInfo *kernel);
MagickBooleanType MagickOrderedPosterizeImageChannel(MagickWand *wand,const ChannelType channel,
  const char *threshold_map);

MagickBooleanType MagickFloodfillPaintImageChannel(
	MagickWand *wand,
	const ChannelType channel, const PixelWand *fill,const double fuzz,
	const PixelWand *bordercolor,const ssize_t x,const ssize_t y,
	const MagickBooleanType invert
);
MagickBooleanType MagickFunctionImageChannel(MagickWand *wand,
  const ChannelType channel,const MagickFunction function,
  const size_t number_arguments,const double *arguments);

MagickWand *MagickFxImageChannel(
	MagickWand *wand,
	const ChannelType channel,
	const char *expression
);

MagickBooleanType MagickGammaImageChannel(MagickWand *wand,
  const ChannelType channel,const double gamma);
MagickBooleanType MagickGaussianBlurImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma);
MagickBooleanType MagickHaldClutImageChannel(MagickWand *wand,
  const ChannelType channel,const MagickWand *hald_wand);
MagickBooleanType MagickLevelImageChannel(MagickWand *wand,
  const ChannelType channel,const double black_point,const double gamma,
  const double white_point);
MagickBooleanType MagickMorphologyImageChannel(MagickWand *wand,
  ChannelType channel,MorphologyMethod method,const ssize_t iterations,
  KernelInfo *kernel);
MagickBooleanType MagickMotionBlurImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma,
  const double angle);
MagickBooleanType MagickNegateImageChannel(MagickWand *wand,
  const ChannelType channel,const MagickBooleanType gray);
MagickBooleanType MagickNormalizeImageChannel(MagickWand *wand,
  const ChannelType channel);
MagickBooleanType MagickOpaquePaintImageChannel(MagickWand *wand,
  const ChannelType channel,const PixelWand *target,
  const PixelWand *fill,const double fuzz,const MagickBooleanType invert);
MagickBooleanType MagickOrderedPosterizeImageChannel(MagickWand *wand,
  const ChannelType channel,const char *threshold_map);
MagickBooleanType MagickRandomThresholdImageChannel(MagickWand *wand,
  const ChannelType channel,const double low,const double high);
MagickBooleanType MagickRotationalBlurImageChannel(MagickWand *wand,
  const ChannelType channel,const double angle);
MagickBooleanType MagickSelectiveBlurImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma,
  const double threshold);
MagickBooleanType MagickSharpenImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma);
MagickBooleanType MagickSigmoidalContrastImageChannel(MagickWand *wand,
  const ChannelType channel,const MagickBooleanType sharpen,
  const double alpha,const double beta);
MagickBooleanType MagickSolarizeImageChannel(MagickWand *wand,
  const ChannelType channel,const double threshold);
MagickBooleanType MagickStatisticImageChannel(MagickWand *wand,
  const ChannelType channel,const StatisticType type,const size_t width,
  const size_t height);
MagickBooleanType MagickThresholdImageChannel(MagickWand *wand,
  const ChannelType channel,const double threshold);
MagickBooleanType MagickUnsharpMaskImageChannel(MagickWand *wand,
  const ChannelType channel,const double radius,const double sigma,
  const double amount,const double threshold);
  
PixelInterpolateMethod MagickGetImageInterpolateMethod(MagickWand *);

MagickBooleanType MagickSetImageInterpolateMethod(MagickWand *wand,
  const PixelInterpolateMethod method);
double DrawGetStrokeOpacity(const DrawingWand *wand);

#endif