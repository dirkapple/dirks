#!/bin/sh
if [ -z $1 ]
then
	echo "empty log file"
	exit -1
fi

awk '{ print gensub(/.\*2014:(.+):.\*+0800\].\*/,"\\\1","g")}' $1 | uniq -c | grep -v Windows | less 
