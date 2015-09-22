#!/bin/sh

#give an error if the download fails.
set -e

imagemagick_fetch_and_build () {
    local version=$1

    echo "version is ${version}"
    im_dir=${HOME}/im/imagemagick-${version}
    cacheable="true"

    case $version in

    git7)
        cacheable="false"
        wget -O ImageMagick-7.tar.gz https://github.com/ImageMagick/ImageMagick/archive/master.tar.gz
        tar xvfz ImageMagick-7.tar.gz
        cd ImageMagick-master
        ;;
    git6)
        cacheable="false"
        wget -O ImageMagick-6.tar.gz https://github.com/ImageMagick/ImageMagick/archive/ImageMagick-6.tar.gz
        tar xvfz ImageMagick-6.tar.gz
        cd ImageMagick-ImageMagick-6
        ;;
    dev)
        cacheable="false"
        svn co https://www.imagemagick.org/subversion/ImageMagick/branches/ImageMagick-6/ imagemagick-dev
        cd imagemagick-dev
        ;;
    *)
        wget "https://launchpad.net/imagemagick/main/${version}/+download/ImageMagick-${version}.tar.gz"
        tar xvfz ImageMagick-${version}.tar.gz
        cd ImageMagick-*
        ;;
    esac

#ignore compile warnings/errors
set +e

    if [ "$cacheable" -eq "false" ] || [ ! -d "${im_dir}" ]; then
        ./configure --prefix="${HOME}/im/imagemagick-${version}" --without-magick-plus-plus --without-perl --disable-openmp --with-gvc=no
        make -j 8
        make install
        cd ..
    else
        echo "Using cached directory ${im_dir}";
    fi
}
imagemagick_fetch_and_build $1