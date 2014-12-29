#!/bin/sh
make clean

/home/s/apps/php/bin/phpize

make

sudo /bin/cp -f /home/libofeng/work/php/php-5.2.6/ext/to_utf8/modules/to_utf8.so /home/s/apps/php/extensions/

echo "done~!"
