--TEST--
Test Imagick, Imagick::exportImagePixels
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x675;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php

$imagick = new \Imagick();
$imagick->newPseudoImage(256, 256, "gradient:black-white");

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

$v = Imagick::getVersion();
if ($v['versionNumber'] < 0x700) {
	//This test will probably fail on 32bit platforms. If you see this please
	//submit a PR that fixes the problem.
	$pixelTypes[Imagick::PIXEL_INTEGER] =  function($v) { return $v / 4294967295; }; 
}



foreach ($pixelTypes as $pixelType => $scaleFn) {
	try {
		$pixels = $imagick->exportImagePixels(0, 0, 1, 256, "R", $pixelType);
	
		for ($i = 0; $i<10 ; $i++) {
			$expectedValue = $i / 255;
			$scaledActualValue = $scaleFn($pixels[$i]);
	
			if (abs($expectedValue - $scaledActualValue) > 0.0001) {
				echo "pixel type $pixelType has incorrect values. They should be 0/255, 1/255, 2/255... 255/255 or the scaled equivalent\n";
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