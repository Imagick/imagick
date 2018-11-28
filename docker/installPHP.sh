#!/usr/bin/env bash


set -e
set -x

# git clone https://github.com/php/php-src

mkdir -p /var/php_src/php-7.2.11
tar -xzf /var/app/php_src/php-7.2.11.tar.gz -C /var/php_src/php-7.2.11 --strip-components=1

cd /var/php_src/php-7.2.11

# ./buildconf
# ./configure --disable-all --without-pear
./configure --without-pear


make -j 4

make install
