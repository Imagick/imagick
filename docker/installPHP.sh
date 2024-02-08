#!/usr/bin/env bash


set -e
set -x

# git clone https://github.com/php/php-src

SRCFILE="php-7.2.11"
SRCFILE="php-7.1.24"

mkdir -p /var/app/php_src/${SRCFILE}
tar -xzf /var/app/php_src/${SRCFILE}.tar.gz -C /var/app/php_src/${SRCFILE} --strip-components=1

cd /var/app/php_src/${SRCFILE}

# ./buildconf
# ./configure --disable-all --without-pear
./configure \
  --without-pear \
  --without-sqlite3 \
  --without-pdo-sqlite \
  --with-openssl \
  --enable-mbstring \
  --with-curl \
  --with-zip




# For Mac


export PKG_CONFIG_PATH="/opt/homebrew/Cellar/openssl@1.1/1.1.1s/lib/pkgconfig:$PKG_CONFIG_PATH"

./configure \
  --without-pear \
  --without-sqlite3 \
  --without-pdo-sqlite \
  --with-openssl \
  --enable-mbstring \
  --with-curl \
  --with-zip \
  --without-iconv \
  --prefix=/opt/danack \
  --exec-prefix=/opt/danack


./configure --with-imagick=/opt/danack




make -j 20

make install


export TEST_PHP_EXECUTABLE=`which php`

# echo "extension=imagick.so" > /usr/local/lib/php.ini