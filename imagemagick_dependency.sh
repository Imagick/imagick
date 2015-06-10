#!/bin/sh

set -e

imagemagick_fetch_and_build () {
    local version=$1

    case $version in
    
    git7)
        wget -O ImageMagick-7.tar.gz https://github.com/ImageMagick/ImageMagick/archive/master.tar.gz
        tar xvfz ImageMagick-7.tar.gz
        cd ImageMagick-master
        ;;
    git6)
        wget -O ImageMagick-6.tar.gz https://github.com/ImageMagick/ImageMagick/archive/ImageMagick-6.tar.gz
        tar xvfz ImageMagick-6.tar.gz
        cd ImageMagick-ImageMagick-6
        ;;
    dev)
        svn co https://www.imagemagick.org/subversion/ImageMagick/branches/ImageMagick-6/ imagemagick-dev
        cd imagemagick-dev
        ;;
    *)
        wget "https://launchpad.net/imagemagick/main/${version}/+download/ImageMagick-${version}.tar.gz"
        tar xvfz ImageMagick-${version}.tar.gz
        cd ImageMagick-*
        ;;
    esac
    ./configure --prefix="${HOME}/imagemagick-${version}" --without-magick-plus-plus --without-perl --disable-openmp
    make -j 8
    make install
    cd ..
}
imagemagick_fetch_and_build $1