<?php

// This is probably a bad idea.
// Depending on system fonts is just not a good approach.
// Any fonts your program relies on, should be packaged and shipped
// with your program, and then loaded explicitly.

$output = '';
$output .= "Fonts that match 'Helvetica*' are:<br>";

$fontList = \Imagick::queryFonts();

if (count($fontList) === 0) {
    echo "There are no fonts available, so something is wrong.\n";
    exit(-1);
}

printf("There are %d fonts:\n", count($fontList));

foreach ($fontList as $fontName) {
    echo  "  " . $fontName . "\n";
}

