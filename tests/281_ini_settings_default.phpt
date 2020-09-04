--TEST--
OpenMP segfault hacks
--SKIPIF--
<?php 


require_once(dirname(__FILE__) . '/skipif.inc');
 
?>
--FILE--
<?php


$sleepCount = ini_get('imagick.shutdown_sleep_count');
$setSingleThread = ini_get('imagick.set_single_thread');

if ($sleepCount != 10) {
    echo "imagick.shutdown_sleep_count is not set to 10 but instead " . var_export($sleepCount, true) ."\n";
}

if ($setSingleThread != 1) {
    echo "imagick.set_single_thread setting is not true but instead " . var_export($setSingleThread, true) ."\n";
}


echo "Complete".PHP_EOL;
?>
--EXPECTF--
Complete
