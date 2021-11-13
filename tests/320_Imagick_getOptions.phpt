--TEST--
Test Imagick, getOptions
--SKIPIF--
<?php
require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('getOptions'));
?>
--FILE--
<?php

function getOptions() {
    $imagick = new \Imagick(__DIR__ . '/Biter_500.jpg');
    $result = $imagick->getOptions();

    if ($result !== []) {
    	echo "unexpected contents of options:\n";
    	var_dump($result);
    }

    $imagick->setOption("jpeg:preserve", "yes");

    $result = $imagick->getOptions();
    $expected = ["jpeg:preserve" => "yes"];

    if ($result !== $expected) {
        echo "unexpected contents of options:\n";
        var_dump($result);
    }

    $imagick->deleteOption("jpeg:preserve");
    $result = $imagick->getOptions();

    if ($result !== []) {
    	echo "unexpected contents of options, failed to delete the set one:\n";
    	var_dump($result);
    }
}

getOptions() ;
echo "Ok";
?>
--EXPECTF--
Ok
