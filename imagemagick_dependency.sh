#!/bin/sh

imagemagick_fetch_and_build () {
    local version=$1

    case $version in
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