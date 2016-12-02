--TEST--
Test Imagick, Imagick::exportImagePixels
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$color = 'rgb(127, 127, 127)';
$width = 5;
$height = 5;
$innerBevel = 3;
$outerBevel = 3;


$imagick = new \Imagick();
$imagick->newPseudoImage(256, 256, "gradient:black-white");

$v = Imagick::getVersion();
if ($v['versionNumber'] < 0x696) {
	$pixelTypes = array(
		Imagick::PIXEL_CHAR => function($v) { return $v / 255; } ,
		Imagick::PIXEL_DOUBLE => function($v) { return $v; } ,
		Imagick::PIXEL_FLOAT => function($v) { return $v; } ,
		Imagick::PIXEL_LONG => function($v) { return $v / 4294967295; },
		Imagick::PIXEL_QUANTUM => function($v) { return $v / Imagick::getQuantum(); } ,
		Imagick::PIXEL_SHORT => function($v) { return $v / 65535; } ,

		// This is not supported as ints close to 64bits are weird in PHP
		// Imagick::PIXEL_LONGLONG => function($v) { return $v / (2 << 64 -1 ); } ,
	);
} else {
	$pixelTypes = array(
		Imagick::PIXEL_CHAR => function($v) { return $v / 256; } ,
		Imagick::PIXEL_DOUBLE => function($v) { return $v; } ,
		Imagick::PIXEL_FLOAT => function($v) { return $v; } ,
		Imagick::PIXEL_LONG => function($v) { return $v / 4294967296; },
		Imagick::PIXEL_QUANTUM => function($v) { return $v / Imagick::getQuantum(); } ,
		Imagick::PIXEL_SHORT => function($v) { return $v / 65536; } ,

		// This is not supported as ints close to 64bits are weird in PHP
		// Imagick::PIXEL_LONGLONG => function($v) { return $v / (2 << 64 ); } ,
	);
}

if ($v['versionNumber'] < 0x700) {
	//This test will probably fail on 32bit platforms. If you see this please
	//submit a PR that fixes the problem.
	if ($v['versionNumber'] < 0x696) {
		$pixelTypes[Imagick::PIXEL_INTEGER] =  function($v) { return $v / 4294967295; };
	} else {
		$pixelTypes[Imagick::PIXEL_INTEGER] =  function($v) { return $v / 4294967296; };
	}
}




foreach ($pixelTypes as $pixelType => $scaleFn) {
	try {
		$pixels = $imagick->exportImagePixels(0, 0, 1, 10, "R", $pixelType);
	
		for ($i = 0; $i<10 ; $i++) {
			if ($v['versionNumber'] < 0x696) {
				$expectedValue = $i / 255;
			} else {
				$expectedValue = $i / 256;
			}
			$scaledActualValue = $scaleFn($pixels[$i]);
	
			if (abs($expectedValue - $scaledActualValue) > 0.0001) {
				if ($v['versionNumber'] < 0x696) {
					echo "pixel type $pixelType has incorrect values. They should be 0/255, 1/255, 2/255....or the scaled equivalent\n";
				} else {
					echo "pixel type $pixelType has incorrect values. They should be 0/256, 1/256, 2/256....or the scaled equivalent\n";
				}
				var_dump($pixels);
				break;
			}
		}
	}
	catch (\Exception $e) {
		echo "Exception caught for pixelType: $pixelType ";
		echo $e->getMessage();
	}
}


echo "Ok";
?>
--EXPECTF--
Ok