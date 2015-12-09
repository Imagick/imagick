
set -x

ls /home/travis/im/imagemagick-${IMAGEMAGICK_VERSION}/lib

for arg in libMagickWand-6.Q16.so libMagickWand.so libMagickWand-7.Q16HDRI.so; do
    filename="/home/travis/im/imagemagick-${IMAGEMAGICK_VERSION}/lib/${arg}"   
    if [ -f "$filename" ]
    then
      nm -gC "$filename" > ./util/symbols.magickwand.txt
    fi
done

for arg in libMagickCore-6.Q16.so libMagickCore.so libMagickCore-7.Q16HDRI.so; do
    filename="/home/travis/im/imagemagick-${IMAGEMAGICK_VERSION}/lib/${arg}"   
    if [ -f "$filename" ]
    then
      nm -gC "$filename" > ./util/symbols.magickcore.txt
    fi
done

if [ ! -f ./util/symbols.magickwand.txt ]; then
    echo "Failed to get symbols for magickwand"
fi

if [ ! -f ./util/symbols.magickcore.txt ]; then
    echo "Failed to get symbols for magickcore"
fi


nm -gC ./modules/imagick.so > ./util/symbols.imagick.txt

# debugging only
# cat ./util/symbols.magickwand.txt
# cat ./util/symbols.magickcore.txt
# cat ./util/symbols.imagick.txt

php ./util/checkSymbols.php
