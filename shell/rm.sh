#!/bin/sh
#删除名字奇怪的文件
for i in $(ls -i | awk '{print $1}')
do
	find ./ -inum $i -exec rm -i {} \;
done
