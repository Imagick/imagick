--TEST--
OpenMP segfault hacks

--INI--
imagick.shutdown_sleep_count=On
imagick.set_single_thread=On
--SKIPIF--
<?php 


require_once(dirname(__FILE__) . '/skipif.inc');
 
?>
--FILE--
<?php


$sleepCount = intval(ini_get('imagick.shutdown_sleep_count'));
$setSingleThread = ini_get('imagick.set_single_thread');

if ($sleepCount != 1) {
    echo "imagick.shutdown_sleep_count is not set to 1 but instead " . var_export($sleepCount, true) ."\n";
}

if ($setSingleThread != 1) {
    echo "imagick.set_single_thread setting is not true but instead " . var_export($setSingleThread, true) ."\n";
}


echo "Complete".PHP_EOL;
?>
--EXPECTF--
Complete
