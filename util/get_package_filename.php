<?php

if ($argc !== 2) {
    echo "Usage php get_package_filename.php 'Package imagick-3.5.0.tgz done'\n";
    exit(-1);
}

// $exampleInput = "Package imagick-3.5.0.tgz done";
// $input = $exampleInput;

$input = $argv[1];
$pattern = "#\w*\s(.*)\s\w*#iu";
$result = preg_match($pattern, $input, $matches);

if ($result !== 1) {
    echo "Failed to extract package file name from $input\n";
    exit(-1);
}

$filename = $matches[1];
$filename_base = str_replace(".tgz", "", $filename);

echo $filename_base;