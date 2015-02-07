--TEST--
Imagick::readImages
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

try {
	$imagick = new Imagick(array (
							'magick:rose',
							'magick:rose',
							'fail_this_does_not_exist.jpg',
							'magick:rose',
	));
	echo 'Fail'. PHP_EOL;
} catch (ImagickException $e) {
	echo 'OK construct exception' . PHP_EOL;
}

$imagick = new Imagick();
$imagick = new Imagick(array (
						'magick:rose',
						'magick:rose',
						'magick:rose',
));
echo 'OK construct' . PHP_EOL;
$imagick->readImages (array (
						'magick:rose',
						'magick:rose',
						'magick:rose',
));
echo 'OK readImages' . PHP_EOL;
try{
	$imagick->readImages (array (
							'magick:rose',
							'fail_this_does_not_exist.jpg',
	));	
	echo 'Fail' . PHP_EOL;
} catch (ImagickException $e) {
	echo 'OK readImages exception'. PHP_EOL;
}

?>
--EXPECT--
OK construct exception
OK construct
OK readImages
OK readImages exception