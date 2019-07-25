
set -e
set -x

scl enable php74 'phpize; ./configure'



scl enable php74 'cd imagick; make -j4; make install'
scl enable php74 'make test TESTS="-q --show-diff"'


