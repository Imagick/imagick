<?
/*
   +----------------------------------------------------------------------+
   | PHP Version 4                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2002 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.02 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | http://www.php.net/license/2_02.txt.                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors:                                                             |
   |                                                                      |
   +----------------------------------------------------------------------+
 */


if(!extension_loaded('imagick')) {
	dl('imagick.so');
}
$handle = imagick_create();



imagick_set_attribute($handle,"size","500x500");
imagick_read($handle,"xc:red");



//print imagick_get_attribute($handle,"width");
//print imagick_get_attribute($handle,"height");
//imagick_oilpaint($handle,20);
//print imagick_get_attribute($handle,"size");
imagick_set_attribute($handle,array("quality"=>1,"magick"=>"JPEG"));


imagick_annotate($handle,array(
"primitive"=>"text 150,150 hello",
            "pointsize"=>60,
            "antialias"=>1,
            "stroke"=>'green',            
            "fill"=>'#ff7755',                        
            "font"=>"Arial.ttf",

            "text"=>"doedl",
            "rotate"=>90
            ));
//imagick_morph($handle,3);


imagick_write($handle,"bla3.png");
//imagick_free($handle);

header("Content-type: image/png");


readfile("bla3.png");



