--TEST--
Allow Imagick exceptions to be extended.
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php
error_reporting( E_ALL ); 

ini_set( "display_errors", true );

class UserlandImagickException extends ImagickException {}
class UserlandImagickDrawException extends ImagickDrawException {}
class UserlandImagickPixelException extends ImagickPixelException {}

if (class_exists('ImagickKernelException', false) == true) {
	class UserlandImagickKernelException extends ImagickKernelException
	{
	}
}

?>
--EXPECTF--
