#!/bin/sh

# This runs the tests in parallel (needs php 7.something)
# and writes the failing tests to a list. Re-running will
# run the failing tests if there were any.
php run-tests.php -j4 -l failing_tests.txt
