FROM debian:11-slim

USER root

# Get Debian up-to-date
RUN apt-get update -qq \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    apt-transport-https autoconf bison bsdmainutils ca-certificates curl \
    gcc git gnupg lsb-release make pkg-config re2c wget \
    libpng-dev libjpeg-dev ghostscript libxml2-dev libfreetype6-dev \
    libonig-dev  libcurl4-openssl-dev libwebp-dev vim \
    strace valgrind libtiff5-dev libfftw3-dev libde265-dev libheif-dev \
    libssl-dev libzip-dev unzip

# Make the default directory a useful one
WORKDIR /var/app

CMD tail -f /var/app/README.md

