<?php

$rc = new ReflectionClass(Imagick::class);

$constants = $rc->getConstants();

$found_prefixes = [];

foreach ($constants as $key => $value) {
    $first_underscore_position = strpos($key, '_');
    if ($first_underscore_position === false) {
        echo "constant found without underscore: $key\n";
        continue;
    }

    $prefix = substr($key, 0, $first_underscore_position);
    if (in_array($prefix, $found_prefixes, true) === false) {
        $found_prefixes[] = $prefix;
    }
}

sort($found_prefixes);

echo "Prefixes are:\n";
foreach ($found_prefixes as $prefix) {
    echo "$prefix\n";
}

$map_of_constants_to_enums = [

	'ALIGN' => "AlignType",
	'ALPHACHANNEL' => "AlphaChannelOption",
	'CHANNEL' => "ChannelType",
	'COLOR' => null, // for some reason we don't use the ImageMagick constants...
	'COLORSPACE' => "ColorspaceType",
	'COMPOSITE' => "CompositeOperator",
	'COMPRESSION' => "CompressionType",
	'DECORATION' => "DecorationType",
	'DIRECTION' => "DirectionType",
	'DISPOSE' => "DisposeType",
	'DISTORTION' => "DistortMethod",
	'DITHERMETHOD' => "DitherMethod",
	'EVALUATE' => "MagickEvaluateOperator", //
	'FILLRULE' => "FillRule",
	'FILTER' => "FilterType",
	'FUNCTION' => "MagickFunction", //
	'GRAVITY' => "GravityType",
	'IMAGICK' => null,// this is internal to extension e.g. PHP_IMAGICK_EXTNUM
	'IMGTYPE' => "ImageType",
	'INTERLACE' => "InterlaceType",
	'INTERPOLATE' => "PixelInterpolateMethod",   //
	'KERNEL' => "KernelInfoType",
	'LAYERMETHOD' => "LayerMethod",
	'LINECAP' => "LineCap",
	'LINEJOIN' => "LineJoin",
	'METRIC' => "MetricType",
	'MONTAGEMODE' => "MontageMode",
	'MORPHOLOGY' => "MorphologyMethod",
	'NOISE' => "NoiseType",
	// 'NORMALIZE_KERNEL' => "GeometryFlags"  - this is not a trivial list of enums...
	'ORIENTATION' => "OrientationType",
	'PAINT' => "PaintMethod",
	'PATHUNITS' => "ClipPathUnits",
	'PIXELMASK' => "PixelMask",
	//	'PIXEL' => 'StorageType', // todo - deprecate this constant.
	'PIXELSTORAGE' => 'StorageType',
	'PREVIEW' => "PreviewType",
	'QUANTUM' => null, // internal to extension e.g.  atoi (MagickQuantumRange)
	'RENDERINGINTENT' => "RenderingIntent",
	'RESOLUTION' => "ResolutionType",
	'RESOURCETYPE' => "ResourceType",
	'SPARSECOLORMETHOD' => "SparseColorMethod",
	'STATISTIC' => "StatisticType",
	'STRETCH' => "StretchType",
	'STYLE' => "StyleType",
	'USE' => null, // internal to extension USE_ZEND_MM
	'VIRTUALPIXELMETHOD' => "VirtualPixelMethod",
];