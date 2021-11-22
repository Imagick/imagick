--TEST--
Test iterating over images works
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

$canvasTypes = [
    "radial-gradient:red-blue",
    "gradient:black-fuchsia",
    "plasma:tomato-steelblue",
];

$canvas = new Imagick();


foreach ($canvasTypes as $canvasType) {
    $imagick = new Imagick();
    $imagick->newPseudoImage(300, 300, $canvasType);
    $canvas->addImage($imagick);
}

$canvas->setImageFormat("png");

foreach ($canvas as $canvasSubImage) {
    echo $canvas->getIteratorIndex() . "\n";
    echo $canvas->getImageIndex() . "\n";
}

for ($i=0; $i<3; $i+=1) {
    $canvas->setImageIndex($i);
    echo $canvas->getIteratorIndex() . "\n";
}
for ($i=0; $i<3; $i+=1) {
    $canvas->setIteratorIndex($i);
    echo $canvas->getImageIndex() . "\n";
}

$canvas->setImageIndex(-1);
if ($canvas->getImageIndex() !== 2) {
    echo "Setting image index to -1 did something unexpected. Or at least is a BC break.\n";
}
else {
    echo "still " . $canvas->getImageIndex() . " as hasn't changed\n";
}

try {
    $canvas->setImageIndex(3);
}
catch (ImagickException $ie) {
    echo "Exception: ". $ie->getMessage() . "\n";
}

echo "Ok";
?>
--EXPECTF--
0
0
1
1
2
2
0
1
2
0
1
2
still 2 as hasn't changed
Exception: Unable to set image index
Ok

