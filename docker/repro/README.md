Repro box is up.

Probably run 'ldconfig /usr/local/lib' so that you can run command line 
programs like 'convert -list font' to check if ImageMagick can find fonts.

You can bash into the container with:

docker-compose exec repro sh

Then install ImageMagick with configure something like:


./configure --with-quantum-depth=16 \
  --with-magick-plus-plus=no \
  --without-perl \
  --disable-docs \
  --with-jpeg=yes \
  --with-png=yes \
  --with-fontconfig=yes \
  --with-webp=yes \
  --with-urw-base35-font-dir=/usr/share/fonts/opentype/urw-base35

And so the long day wears on.