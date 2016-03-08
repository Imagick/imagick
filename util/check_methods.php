<?php

$tests = [
	"draw",
	"pixel",
	"wand"
];

$unsupportedMethods = [
	"MagickSetImageEndian", // nope.
	"MagickGetImageEndian", //nope, nope, nope
	"MagickDestroyImage", //this shouldn't be needed. It's covered by clear?
	"MagickGetImageFuzz", // I'm not aware of any operations that use the built in fuzz
	"MagickSetImageFuzz", // I'm not aware of any operations that use the built in fuzz
	"MagickGetImageChannelDistortions", //Don't understand
	"MagickGetImageChannelFeatures", //Don't understand
	"MagickConstituteImage", //duplicate functionality.
	"MagickCompositeLayers", //duplicate functionality.

	//Pixel
	"PixelGetException", // not a user function
	"NewPixelWands", // Probably not needed.
	"IsDrawingWand", //Not needed
	"ClonePixelWands", // Hopefully not needed
	"DestroyPixelWands", //Shouldn't be needed?
	"PixelGetMagickColor", // We don't expose MagickPixelPacket currently
	"PixelSetMagickColor", // We don't expose MagickPixelPacket currently
	"ClonePixelWands", //Shouldn't be needed.
	"DestroyPixelWands", //shouldn't be needed.
	"PixelGetColorAsNormalizedString", //stahp
	"PixelGetExceptionType", // This should be everywhere?
	"PixelGetQuantumColor", //pixelpackets are not exposed.
	"PixelSetQuantumColor", //pixelpackets are not exposed.

	// Draw
	"DrawAllocateWand", // not a user function
	"IsDrawingWand", //Not needed
];


$methods = [];

foreach ($tests as $test) {
	$lines = file($test."_methods.txt");
	if ($lines === false) {
		echo "Couldn't read file for $test \n"; 
		exit(-1);
	}

	foreach ($lines as $line) {
		$methods[trim($line)] = false;
	}
}



$files = glob("../*.c");

$contents = [];

$excludeSourceFiles = [
	"#.*shim.s*#"
];

foreach ($files as $file) {
	//Don't look in some source files.
	foreach ($excludeSourceFiles as $excludeSourceFile) {
		if (preg_match($excludeSourceFile, $file) !== 0) {
			echo "Excluding file $file\n";
			continue 2;
		}
	}

	echo "Check file $file \n";

	$contents = file_get_contents($file);

	if ($contents == false) {
		echo "Failed to read $file\n";
		exit(0);
	}

	foreach ($methods as $name => $found) {
		if ($found == true) {
			continue;
		}

		$position = strpos($contents, $name);

		if ($position !== false) {
			$methods[$name] = true;
		}
	}
}

//exit(0);

foreach ($methods as $name => $found) {
	if (in_array($name, $unsupportedMethods) == true) {
		continue;
	}

	if ($found == false) {
		echo "Missing: $name\n";
	}
}









