<?php

// Run with something like:
// export CFLAGS=$(php util/calculate_cflags.php "8.0" "7.0.1-0")
// where the first number is the PHP version, and the second number
// is the ImageMagick version number

//export CFLAGS=$(php util/calculate_cflags.php "7.3" "7.0.10-27")

if ($argc !== 3) {
    fwrite(STDERR, "usage php calculate_cflags.php \$PHP_VERSION \$IMAGEMAGICK_VERSION \n");
    exit(-1);
}

$PHP_VERSION = $argv[1];
$IMAGEMAGICK_VERSION = $argv[2];

$message = sprintf(
    "Calculating for PHP_VERSION [%s] IMAGEMAGICK_VERSION [%s]\n",
    $PHP_VERSION,
    $IMAGEMAGICK_VERSION
);
fwrite(STDERR, $message);

$CFLAGS = array(
    "-Wno-deprecated-declarations",
    "-Wdeclaration-after-statement",
    "-Werror",
    "-Wall",
    "-Wextra",
    "-Wimplicit-function-declaration",
    "-Wno-variadic-macros",
//    "-Wno-sign-compare",
);

if (strpos($IMAGEMAGICK_VERSION, "6.8") === 0) {
    $CFLAGS = array("-Wno-deprecated-declarations");
}

if (strpos($IMAGEMAGICK_VERSION, "6.9") === 0 ||
    strpos($PHP_VERSION, "7") === 0) {
    $CFLAGS[] = "-Wno-unused-parameter";
}

if (strpos($IMAGEMAGICK_VERSION, "6.9") === 0) {
    $CFLAGS[] = "-Wno-unused-variable";
}

if ($PHP_VERSION == "5.4" || $PHP_VERSION == "5.5") {
    $CFLAGS = array("-Wno-deprecated-declarations");
}
else if ($PHP_VERSION == "5.6") {
    $CFLAGS = array(
        "-Wno-deprecated-declarations",
        "-Wdeclaration-after-statement",
        "-Werror",
        "-Wall",
        "-Wimplicit-function-declaration"
    );
}

$result = implode(" ", $CFLAGS);
echo $result;
fwrite(STDERR, "Ok. CFLAGS are: $result\n");
