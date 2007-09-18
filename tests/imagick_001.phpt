--TEST--
Imagick::readImage tests
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php
print "--- Catch exception with try/catch\n";
$imagick = new Imagick();
try {
	$imagick->readImage('foo.jpg');
} catch (ImagickException $e) {
	var_dump($e);
}
print "--- Don't catch exception with try/catch\n";
$imagick->readImage('foo.jpg');

?>
--EXPECTF--
--- Catch exception with try/catch
object(ImagickException)#%d (6) {
  ["message:protected"]=>
  string(%d) "unable to open image `%s': No such file or directory"
  ["string:private"]=>
  string(0) ""
  ["code:protected"]=>
  int(1)
  ["file:protected"]=>
  string(%d) "%s"
  ["line:protected"]=>
  int(5)
  ["trace:private"]=>
  array(1) {
    [0]=>
    array(6) {
      ["file"]=>
      string(%d) "%s"
      ["line"]=>
      int(5)
      ["function"]=>
      string(9) "readimage"
      ["class"]=>
      string(7) "Imagick"
      ["type"]=>
      string(2) "->"
      ["args"]=>
      array(1) {
        [0]=>
        string(7) "foo.jpg"
      }
    }
  }
}
--- Don't catch exception with try/catch

Fatal error: Uncaught exception 'ImagickException' with message 'unable to open image `%s': No such file or directory' in %s:%d
Stack trace:
#0 %s(%d): Imagick->readimage('foo.jpg')
#1 {main}
  thrown in %s on line %d
