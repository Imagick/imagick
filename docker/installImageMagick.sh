#!/usr/bin/env bash

set -e
set -x

# git clone https://github.com/ImageMagick/ImageMagick.git

SRCFILE="ImageMagick-7.0.8-14"
SRCFILE="ImageMagick-7.0.1-0"
SRCFILE="ImageMagick-7.0.4-9"


tar -xzf /var/app/imagemagick_src/${SRCFILE}.tar.gz -C /var/app/imagemagick_src

cd /var/app/imagemagick_src/${SRCFILE}

./configure --with-quantum-depth=16 \
           --with-magick-plus-plus=no \
           --without-perl \
           --disable-static \
           --disable-docs \
           --disable-openmp \
           --with-jpeg=yes \
           --with-png=yes

make install -j 4

