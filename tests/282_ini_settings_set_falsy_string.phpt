--TEST--
OpenMP segfault hacks

--INI--
imagick.shutdown_sleep_count=Off
imagick.set_single_thread=0
--SKIPIF--
<?php 


require_once(dirname(__FILE__) . '/skipif.inc');
 
?>
--FILE--
<?php


// So, this can't be tested for properly. ini values are stored as strings internally
// to PHP, and are not normalised to the actual type used by an extension. Which means
// you can't easily get the actual value being used by an extension, when the input
// type isn't the same type as the extension is going to use it as.
// aka 'Off' is stored as '' not 0.
//
//$sleepCount = ini_get('imagick.shutdown_sleep_count');
//if ($sleepCount !== 0) {
//    echo "imagick.shutdown_sleep_count is not set to 0 but instead " . var_export($sleepCount, true) ."\n";
//}

$setSingleThread = ini_get('imagick.set_single_thread');

// This should be a strict compare but can't be because
// it's stored as a string...
if ($setSingleThread != 0) {
    echo "imagick.set_single_thread setting is not 0 but instead " . var_export($setSingleThread, true) ."\n";
}


echo "Complete".PHP_EOL;
?>
--EXPECTF--
Complete
