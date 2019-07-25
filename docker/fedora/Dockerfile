FROM fedora:30

RUN dnf -y update

RUN dnf -y install 'dnf-command(config-manager)'
RUN dnf -y install https://rpms.remirepo.net/fedora/remi-release-30.rpm
RUN dnf config-manager --set-enabled remi
RUN dnf config-manager --set-enabled remi-debuginfo
RUN dnf -y install php74 php74-php-devel php74-php-debuginfo ImageMagick-devel gdb git make valgrind


WORKDIR /home/app

#RUN scl enable php74 'php -v'
#RUN git clone https://github.com/Imagick/imagick.git
#RUN scl enable php74 'cd imagick; phpize; ./configure'
# RUN scl enable php74 'cd imagick; make -j4; make test TESTS="-q --show-diff"'

# RUN scl enable php74 'cd imagick; make -j4; make install'
# RUN scl enable php74 'make test TESTS="-q --show-diff"' || ls -l /imagick

CMD tail -f -n 1000 /home/app/docker/fedora/debug_notes.txt