--TEST--
Bug #66098	Check that there is no segfault from zval_addref_p
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc');
--FILE--
<?php

$img = new Imagick();
echo $img->foobar;

echo "OK";

?>
--EXPECT--
OK