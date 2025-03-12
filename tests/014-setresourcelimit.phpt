--TEST--
Imagick::setResourceLimit test
--SKIPIF--
<?php 
$imageMagickRequiredVersion=0x692;
require_once(dirname(__FILE__) . '/skipif.inc'); 
?>
--FILE--
<?php


$k = 1024;
$m = $k * $k;

// These tests are flaky as the values ImageMagick will accept
// are limited by the policy.xml of the system.
// Also, it appears that some versions of ImageMagick will
// reject overly large values. e.g. setting RESOURCETYPE_WIDTH
// to a billion fails. Which is not totally unreasonable.

$tests = array(
	Imagick::RESOURCETYPE_AREA =>  100000000,

	// Set maximum amount of disk space in bytes permitted for use by the pixel cache. When this limit is exceeded, the pixel cache is not be created and an error message is returned.
	Imagick::RESOURCETYPE_DISK =>  100,

	//Set maximum number of open pixel cache files. When this limit is exceeded, any subsequent pixels cached to disk are closed and reopened on demand. This behavior permits a large number of images to be accessed simultaneously on disk, but with a speed penalty due to repeated open/close calls.
	Imagick::RESOURCETYPE_FILE => 100,

	// Set maximum amount of memory map in bytes to allocate for the pixel cache. When this limit is exceeded, the image pixels are cached to disk
	Imagick::RESOURCETYPE_MAP => 123 * $m,

	// Set maximum amount of memory in bytes to allocate for the pixel cache from the heap. When this limit is exceeded, the image pixels are cached to memory-mapped disk
	Imagick::RESOURCETYPE_MEMORY => 234 * $m,
);

if (defined('Imagick::RESOURCETYPE_TIME')) {
	$tests[Imagick::RESOURCETYPE_TIME] = 30;
}

if (defined('Imagick::RESOURCETYPE_THROTTLE')) {
	$tests[Imagick::RESOURCETYPE_THROTTLE] = 1;
}
if (defined('Imagick::RESOURCETYPE_THREAD')) {
	$tests[Imagick::RESOURCETYPE_THREAD] = 1;
}
if (defined('Imagick::RESOURCETYPE_WIDTH')) {
	$tests[Imagick::RESOURCETYPE_WIDTH] = 15 * $k;
}
if (defined('Imagick::RESOURCETYPE_HEIGHT')) {
	$tests[Imagick::RESOURCETYPE_HEIGHT] = 15 * $k;
}

$reflection_class = new ReflectionClass(Imagick::class);
$constants = $reflection_class->getConstants();
$resource_constants = [];
foreach ($constants as $name => $value) {
    if (strpos($name, "RESOURCETYPE") === 0) {
        $resource_constants[$value] = $name;
    }
}


foreach ($tests as $resourceType => $value) {
	Imagick::setResourceLimit($resourceType, $value);
	$actualValue = Imagick::getResourceLimit($resourceType);

	if ($actualValue != $value) {
		$resourceTypeString = $resource_constants[$resourceType];
		echo "Error testing $resourceTypeString, value returned $actualValue is not $value \n";
	}
}

echo 'success';

?>
--EXPECTF--
success