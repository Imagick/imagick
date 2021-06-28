<?php

if (PHP_MAJOR_VERSION === 5) {
    echo "";
    exit();
}

if (PHP_MAJOR_VERSION === 7 && PHP_MAJOR_VERSION >= 4) {
    echo "";
    exit();
}

echo "-j4";
