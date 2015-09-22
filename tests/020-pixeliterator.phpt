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

function count_objects ($pix)
{
	$objects = 0;

	foreach ($pix as $r)
		foreach ($r as $o)
			$objects++;

	return $objects;
}

function count_objects_with_iterator ($pixelRegion)
{
	$objects = 0;
	$row = 0;

	$pixelRegion->rewind();
	$pixelRegion->resetIterator();

	while($pixelRow = $pixelRegion->current()) {
		$row++;
		foreach ($pixelRow as $pixel) {
			$objects++;
		}

		$pixelRegion->syncIterator();
		$pixelRegion->next();
		if (!$pixelRegion->valid()) {
			break;
		}
	}

	return $objects;
}

$im = new Imagick ('magick:rose');
$it1 = new ImagickPixelIterator ($im);
$it2 = ImagickPixelIterator::getPixelIterator ($im);
$it3 = $im->getPixelIterator();


$count1 = count_rows ($it1);
$count2 = count_rows ($it2);
$count3 = count_rows ($it3);

if ($count1 != $count2 || 
    $count1 != $count3) {
    printf(
        "Row counts do not match %d %d %d",
        $count1,
        $count2,
        $count3
    );
}

if ($count1 != $count2 || 
    $count1 != $count3) {
    printf(
        "Object counts do not match %d %d %d",
        $count1,
        $count2,
        $count3
    );
}

$objects = array($it1, $it2, $it3);

foreach ($objects as $object) {
	$loop = 0;
	$count = count_objects($object);
	$countIterator = count_objects_with_iterator($object);
	if ($countIterator != $count) {
    	echo "Counting with iterator doesn't match counting with foreach $loop, $count != $countIterator.";
    	$loop++;
	}
}


$it1->newPixelIterator (new Imagick ('magick:rose'));

echo 'done' . PHP_EOL;
?>
--EXPECTF--

%s: ImagickPixelIterator::newPixelIterator is deprecated. ImagickPixelIterator::getPixelIterator should be used instead in %s on line %d
done