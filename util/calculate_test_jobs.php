<?php

// From PHP 7.4, run-tests.php can accept a -jx parameter to
// run tests in parallel. This script just does a version check
// to see if it can be used.

if (PHP_MAJOR_VERSION === 5) {
    echo "";
    exit();
}

if (PHP_MAJOR_VERSION === 7 && PHP_MAJOR_VERSION >= 4) {
    echo "";
    exit();
}

echo "-j4";
