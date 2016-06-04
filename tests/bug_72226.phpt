--TEST--
Imagick don't borg the error handler
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php
error_reporting( E_ALL ); 

ini_set( "display_errors", true );

class UserlandImagickException extends ImagickException {}

?>
--EXPECTF--
