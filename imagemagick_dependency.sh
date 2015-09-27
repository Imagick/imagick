#!/bin/sh

#give an error if the download fails.
set -e

imagemagick_fetch_and_build () {
    local version=$1

    echo "version is ${version}"
    im_dir=${HOME}/im/imagemagick-${version}

    case $version in
        git7)
            ;;
        git6)
            ;;
        dev)
            ;;
        *)
    # Both individual commits, and tagged versions are cacheable
        if [ -d "${im_dir}" ]; then
            echo "Using cached directory ${im_dir}"
            return  
        fi
        ;;
    esac


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
        echo "version is ${version}"
        set +e
        #this can error
        start_str=${version:0:6}
        set -e
        
        if [ "${start_str}" == "commit" ]; then
        
            sha=${version:7:47}
            wget -O "ImageMagick-${sha}.tar.gz" "https://github.com/ImageMagick/ImageMagick/archive/${sha}.tar.gz"
            tar xvfz ImageMagick-${sha}.tar.gz
            
            cd "ImageMagick-${sha}"
        else
           wget "https://launchpad.net/imagemagick/main/${version}/+download/ImageMagick-${version}.tar.gz"
           tar xvfz ImageMagick-${version}.tar.gz
           cd ImageMagick-*
        fi
        ;;
    esac

#ignore compile warnings/errors
set +e


    ./configure --prefix="${HOME}/im/imagemagick-${version}" --without-magick-plus-plus --without-perl --disable-openmp --with-gvc=no
    make -j 8
    make install
    cd ..

}

imagemagick_fetch_and_build $1