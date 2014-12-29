#!/bin/sh
make clean

/home/s/apps/php/bin/phpize

make

sudo /bin/cp -f /home/libofeng/work/php/php-5.2.6/ext/cntwd/modules/cntwd.so /home/s/apps/php/extensions/

echo "done~!"

