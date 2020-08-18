<?php



$output = '';
$output .= "Fonts that match 'Helvetica*' are:<br>";

$fontList = \Imagick::queryFonts();

//var_dump($fontList);

if (count($fontList) === 0) {
    echo "There are no fonts available, so something is wrong.\n";
    exit(-1);
}

printf("There are %d fonts:\n", count($fontList));

foreach ($fontList as $fontName) {
    echo  "  " . $fontName . "\n";
}

