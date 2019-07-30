
set -e
set +x

for ((i=1;i<=5000;i++));
do
  echo $i
  /opt/remi/php74/root/usr/bin/php -d extension=/opt/remi/php74/root/usr/lib64/php/modules/imagick.so /home/app/tests/003_cast_color_opacity.php > /dev/null

  retVal=$?
  if [ $retVal -ne 0 ]; then
    echo "Error"
  fi
done

