#!/bin/sh


#########################################################
# How to run git bisect
#########################################################

## Check out ImageMagick
# git clone https://github.com/ImageMagick/ImageMagick.git

## Put a known good version of Imagick into the directory /var/app/bisect/ImageMagick/imagick

## Update this script to run the relevant tests
## e.g. change php run-tests.php tests/316_Imagick_getImageKurtosis.phpt

## Start bisect session
# git bisect start

## Find bad version of ImageMagick e.g. a particular tag that is broken
# git checkout 963f5fa

## Mark that version as bad.
# git bisect bad


## Find a good version
# git checkout 99da019

## Mark that version as good.
# git bisect good


## Run the bisect automatically
# git bisect run sh bisect_analyze.sh


cd /var/app/bisect/ImageMagick

./configure --with-quantum-depth=16 \
  --disable-dependency-tracking \
  --with-magick-plus-plus=no \
  --without-perl \
  --disable-docs \
  --with-openexr=yes \
  --with-fontconfig=yes \
  --with-fftw \
  --with-heic=yes \
  --with-jpeg=yes \
  --with-png=yes \
  --with-tiff=yes \
  --with-urw-base35-font-dir=/usr/share/fonts/type1/urw-base35 \
  --with-webp=yes

make clean
make install -j20

cd /var/app/bisect/ImageMagick/imagick
phpize
./configure
make install
php run-tests.php tests/316_Imagick_getImageKurtosis.phpt