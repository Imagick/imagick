--TEST--
Pixel Iterator tests
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

function count_rows ($pix)
{
	$rows = 0;
	
	foreach ($pix as $r)
		$rows++;
		
	return $rows;
}

$im = new Imagick ('magick:rose');
$it1 = new ImagickPixelIterator ($im);

$it2 = ImagickPixelIterator::getPixelIterator ($im);
echo (count_rows ($it1) == count_rows ($it2) ? "match" : "no") . PHP_EOL;

$it1->newPixelIterator (new Imagick ('magick:rose'));

echo 'done' . PHP_EOL;
?>
--EXPECTF--
match

%s: ImagickPixelIterator::newPixelIterator is deprecated. ImagickPixelIterator::getPixelIterator should be used instead in %s on line %d
done