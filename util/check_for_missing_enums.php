<?php

// A simple tool to scan for missed enum entries in the ImageMagick header files.
// This code does not attempt to maintain BC across all ImageMagick versions. It
// only needs to work against the latest version of ImageMagick 6 + 7.

$directory = false; 
// Either pass in the directory to scan or set it here.
// $directory = "/home/github/BastionRPM/zips/ImageMagick/ImageMagick-7.0.1-9";
// 'php ./scanHeaders.php ../imagemagick_src/ImageMagick-7.0.11-12'

if ($directory) {
	$pathToImageMagick = $directory;
}
else {
	if ($argc != 2) {
		echo "usage 'php scanHeaders.php /path/to/ImageMagick\n";
		exit(-1);
	}

	$pathToImageMagick = $argv[1];
}

$pathToImageMagick .= '/';

echo "pathToImageMagick is allegedly $pathToImageMagick\n";

if (file_exists($pathToImageMagick) == false || 
	is_dir($pathToImageMagick) == false) {
	echo "Path $pathToImageMagick is not accessible.\n";
	exit(-1);
}

$includeDir = null;

$directory = realpath($pathToImageMagick);

if ($directory === false) {
    echo "Path $pathToImageMagick isn't a real path?\n";
    exit(-1);
}

// IM6 checks
if (file_exists($directory . '/'.'magick') == true) {
	$enumToCheck = [
		'magick/compare.h' => [
			'MetricType',
		],
		'magick/colorspace.h' => [
			'ColorspaceType',
		],
		'magick/composite.h' => [
			'CompositeOperator',
		],
		'magick/compress.h' => [
			'CompressionType',
		],
		'magick/distort.h' => [
			'DistortImageMethod',
			'SparseColorMethod',
		],
		'magick/effect.h' => [
			'PreviewType',
		],
		'magick/fourier.h' => [
			// 'ComplexOperator', // used by ComplexImages - not exposed in wand api
		],
		'magick/fx.h' => [
			'NoiseType',
		],
		'magick/geometry.h' => [
			'GravityType',
		],
		'magick/image.h' => [
			'AlphaChannelType',
			'ImageType',
			'InterlaceType',
			'OrientationType',
			'ResolutionType',
			//'TransmitType',
		],
		'magick/layer.h' => [
			'DisposeType',
			'ImageLayerMethod',
		],
		'magick/magick-type.h' => [
			'ChannelType',
		],
		'magick/morphology.h' => [
			'KernelInfoType',
			'MorphologyMethod'
		],
		'magick/pixel.h' => [
			'InterpolatePixelMethod',
			// 'PixelComponent', // present in 6.9.2 but not used?
			// 'PixelIntensityMethod', //// Used by GrayscaleImage function that is not expose in wand api
		],
		'magick/profile.h' => [
			'RenderingIntent',
		],
		'magick/quantize.h' => [
			'DitherMethod',
		],
		'magick/resample.h' => [
			'FilterTypes',
		],
		'magick/resource_.h' => [
			'ResourceType'
		],
		'magick/statistic.h' => [
			'MagickEvaluateOperator',
			'MagickFunction',
			'StatisticType',
		],
		'magick/type.h' => [
			'StretchType',
			'StyleType',
		],
	];
}
// IM7 checks
else if (file_exists($directory.'/'.'MagickCore') == true) {
	$enumToCheck = [
		'MagickCore/cache-view.h' => [
			'VirtualPixelMethod',
		],
		'MagickCore/compare.h' => [
			'MetricType',
		],
		'MagickCore/colorspace.h' => [
			'ColorspaceType',
		],
		'MagickCore/composite.h' => [
			'CompositeOperator',
		],
		'MagickCore/compress.h' => [
			'CompressionType',
		],
		'MagickCore/distort.h' => [
			'DistortMethod',
			'SparseColorMethod',
		],
		'MagickCore/draw.h' => [
			'AlignType',
			"ClipPathUnits",
			'DecorationType',
			'DirectionType',
			'FillRule',
			'LineCap',
			'LineJoin',
		],
		'MagickCore/effect.h' => [
			'PreviewType',
		],
		'MagickCore/fourier.h' => [
			'ComplexOperator', // used by ComplexImages - not exposed in wand api
		],
		'MagickCore/geometry.h' => [
			// 'GeometryFlags', this is a nightmare of an 'enum'
			'GravityType',
		],
		'MagickCore/image.h' => [
			'AlphaChannelOption',
			'ImageType',
			'InterlaceType',
			'OrientationType',
			'ResolutionType',
			//'TransmitType',
		],
		'MagickCore/layer.h' => [
			'DisposeType',
			'LayerMethod',
		],
		'MagickCore/morphology.h' => [
			'KernelInfoType',
			'MorphologyMethod'
		],
		'MagickCore/pixel.h' => [
			'ChannelType',
			'PixelInterpolateMethod',
			// 'PixelComponent', // present in 6.9.2 but not used?
			// 'PixelIntensityMethod', //// Used by GrayscaleImage function that is not expose in wand api

            //'PixelTrait', no idea
            'PixelMask',
            // 'StorageType', This needs to be re-enabled, but also involves work getting 64bits to play nicely...
		],
		'MagickCore/profile.h' => [
			'RenderingIntent',
		],
		'MagickCore/quantize.h' => [
			'DitherMethod',
		],
		'MagickCore/resample.h' => [
			'FilterType',
		],
		'MagickCore/resource_.h' => [
			'ResourceType'
		],
		'MagickCore/statistic.h' => [
			'MagickEvaluateOperator',
			'MagickFunction',
			'StatisticType',
		],
		'MagickCore/type.h' => [
			'StretchType',
			'StyleType',
		],
		'MagickCore/threshold.h' => [
			'AutoThresholdMethod'
		],
		'MagickCore/visual-effects.h' => [
			'NoiseType',
		],
	];
}
else {
    echo "oops - couldn't figure out enums to check.\n";
    echo "Check what directories exist in $directory \n";
    system("ls -l $directory");

    exit(-1);
}


// Read the current list of enums in Imagick
$imagickHelperContents = file_get_contents(__DIR__ . "/../imagick_helpers.c");
if ($imagickHelperContents == false) {
	echo "failed to read ../imagick_helpers.c\n";
	exit(-1);
}

// We don't use these enums currently.
$skipEnumList = [
//	'ComplexOperator', // used by ComplexImages - not exposed in wand api
//	'PixelComponent', // present in 6.9.2 but not used?
//	'PixelIntensityMethod', // Used by GrayscaleImage function that is not expose in wand api
];

$any_missing = false;

foreach ($enumToCheck as $filename => $enums) {
	foreach ($enums as $enum) {
		if (in_array($enum, $skipEnumList)) {
			//We don't use these enums currently.
			continue;
		}

		$enumNameList = getEnumList($enum, $pathToImageMagick.$filename);
		foreach ($enumNameList as $enumName) {
			//echo "checking for $enumName\n";
			if (stripos($imagickHelperContents, $enumName) === false) {
				echo "value '$enumName' is missing for enum $enum.\n";
				$any_missing = true;
			}
		}
	}
}


if ($any_missing === true) {
    exit(-1);
}
exit(0);

function getEnumList($enum, $filename)
{
	$fileContents = file_get_contents($filename);

	if ($fileContents === false) {
		echo "Could not read file $filename \n";
		exit(-1);
	}

	// These characters mess with the matching.
	$searchReplace = [
		','
	];

	$pregString = '#\{([^\{\}]*)\}\s*'.$enum.'#sU';

	preg_match($pregString, $fileContents, $matches);

	if (count($matches) == 0) {
		echo "Couldn't find enum $enum in $filename.\n";
		return [];
	}

	$match = $matches[1];

	foreach ($searchReplace as $search) {
		$match = str_replace($search, '', $match);
	}

	$match = preg_replace('#/\*.*\*/#sU', '', $match);

	$vars = explode("\n", $match);

	$trimmedVars = [];

	foreach ($vars as $var) {
		if (stripos($var, 'Undefined') !== false) {
			continue;
		}
		$equalsPosition = strpos($var, '=');
		if ($equalsPosition !== false) {
			$var = substr($var, 0, $equalsPosition);
		}

		$entry = trim($var);

		if (strlen($entry) === 0) {
			continue;
		}

		if (strpos($entry, '#') !== false) {
			// skip things like '#undef EvenOddRule'
			continue;
		}

		$trimmedVars[] = $entry;
	}

	return $trimmedVars;
}
