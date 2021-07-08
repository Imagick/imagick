<?php

if ($argc !== 2) {
    echo "Usage php get_package_filename.php 'Package imagick-3.5.0.tgz done'\n";
    exit(-1);
}
 $input = $argv[1];

//$exampleInput = <<< EXAMPLE
//Warning: Channel validator warning: field "date" - Release Date "2021-06-17" is not today
//Package imagick-3.5.0.tgz done
//EXAMPLE;
//
//$input = $exampleInput;


$lines = explode("\n", $input);
$line = $lines[count($lines) - 1];

$pattern = "#\w*\s(.*)\s\w*#iu";
$result = preg_match($pattern, $line, $matches);

if ($result !== 1) {
    echo "Failed to extract package file name from $input\n";
    exit(-1);
}

$filename = $matches[1];

if (strpos($filename, ".tgz") === false) {
    echo "Error: alleged filename [$filename] does not contain '.tgz'.\n";
    exit(-1);
}

$filename_base = str_replace(".tgz", "", $filename);

echo $filename_base;