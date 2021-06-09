FROM debian:latest AS env

USER root

RUN echo 'deb http://deb.debian.org/debian testing main' >> /etc/apt/sources.list

# Get Debian up-to-date
RUN apt-get update -qq \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    apt-transport-https autoconf bison bsdmainutils ca-certificates curl \
    gcc git gnupg lsb-release make pkg-config re2c wget \
    libpng-dev libjpeg-dev ghostscript libxml2-dev libfreetype6-dev \
    libonig-dev libwebp-dev vim strace valgrind \
    libssl-dev libcurl4-openssl-dev fontconfig fonts-urw-base35

RUN echo "Probably run 'ldconfig /usr/local/lib' so that you can run command line programs like "
RUN echo "'convert -list font' to check if ImageMagick can find fonts."
RUN echo ""
RUN echo "****************************"
RUN echo "****************************"

# Make the default directory you
WORKDIR /var/app

CMD tail -f -n 1000 /var/app/docker/repro/README.md

