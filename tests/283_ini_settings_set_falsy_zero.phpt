--TEST--
OpenMP segfault hacks

--INI--
imagick.shutdown_sleep_count=0
imagick.set_single_thread=0
--SKIPIF--
<?php 


require_once(dirname(__FILE__) . '/skipif.inc');
 
?>
--FILE--
<?php


$sleepCount = ini_get('imagick.shutdown_sleep_count');
$setSingleThread = ini_get('imagick.set_single_thread');

if ($sleepCount != 0) {
    echo "imagick.shutdown_sleep_count is not set to 10 but instead " . var_export($sleepCount, true) ."\n";
}

if ($setSingleThread != 0) {
    echo "imagick.set_single_thread setting is not false but instead " . var_export($sleepCount, true) ."\n";
}


echo "Complete".PHP_EOL;
?>
--EXPECTF--
Complete
