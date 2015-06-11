--TEST--
Test ImagickPixelIterator, construct
--SKIPIF--
<?php

require_once(dirname(__FILE__) . '/skipif.inc');

checkClassMethods('Imagick', array('getHDRIEnabled'));


?>
--FILE--
<?php

$enabled = Imagick::getHDRIEnabled();

if ($enabled === true || $enabled === false) {
    echo "Ok";
}
else {
    echo "Unexpected value for Imagick::getHDRIEnabled:\n";
    var_dump($enabled);
}

?>
--EXPECTF--
Ok