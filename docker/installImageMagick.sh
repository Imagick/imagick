#!/usr/bin/env bash

set -e
set -x

# git clone https://github.com/ImageMagick/ImageMagick.git

SRCFILE="ImageMagick-7.0.8-14"
# SRCFILE="ImageMagick-7.0.1-0"
# SRCFILE="ImageMagick-7.0.4-9"
SRCFILE="ImageMagick-7.0.10-27"

tar -xzf /var/app/imagemagick_src/${SRCFILE}.tar.gz -C /var/app/imagemagick_src

cd /var/app/imagemagick_src/${SRCFILE}

./configure --with-quantum-depth=16 \
           --with-magick-plus-plus=no \
           --without-perl \
           --disable-docs \
           --with-jpeg=yes \
           --with-png=yes \
           --with-webp=yes

# --disable-openmp \
#   --libdir=/usr/local/lib \

# --enable-hdri
# --libdir=/usr/local/lib
# /lib/tls/x86_64/
# /usr/lib/tls
# --disable-openmp \

ln -s /usr/local/lib/libMagickCore-7.Q16HDRI.so.0 /usr/lib/x86_64-linux-gnu/libMagickCore-7.Q16HDRI.so.0
ln -s /usr/local/lib/libMagickWand-7.Q16HDRI.so.0 /usr/lib/x86_64-linux-gnu/libMagickWand-7.Q16HDRI.so.0


make install -j 4

