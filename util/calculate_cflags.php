<?php

// Run with something like:
// export CFLAGS=$(php util/calculate_cflags.php "8.1" "7.1.0-17")
// where the first number is the PHP version, and the second number
// is the ImageMagick version number

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
    "-Wformat"
);

if (strpos($IMAGEMAGICK_VERSION, "7.1.1") === 0 ||
    strpos($IMAGEMAGICK_VERSION, "git7") === 0) {
    // The definition of channel changed to be a non-negative number, which
    // results in this code:
    //
    // if (((ssize_t) channel < 0) || ((ssize_t) channel >= MaxPixelChannels))
    //
    // giving this error:
    // error: comparison is always false due to limited range of data type [-Werror=type-limits]
    $CFLAGS[] = "-Wno-type-limits";
}

if (strpos($IMAGEMAGICK_VERSION, "6") === 0 ||
    strpos($IMAGEMAGICK_VERSION, "git6") === 0) {
    $CFLAGS[] = "-Wno-sign-compare";
    $CFLAGS[] = "-Wno-unused-parameter";
}

if (strpos($IMAGEMAGICK_VERSION, "6.8") === 0) {
    $CFLAGS = array("-Wno-deprecated-declarations");
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

// Problem with zend_*.h files on newer PHP builds
if (in_array($PHP_VERSION, ['8.2', '8.3', '8.4'], true)) {
    $CFLAGS = array_values(array_diff($CFLAGS, ['-Wdeclaration-after-statement']));
}
if (in_array($PHP_VERSION, ['8.4'], true)) {
    $CFLAGS[] = "-Wno-missing-field-initializers";
}

$result = implode(" ", array_unique($CFLAGS));
echo $result;
fwrite(STDERR, "Ok. CFLAGS are: $result\n");
