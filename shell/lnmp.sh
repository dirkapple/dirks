#!/bin/bash

DETAIL=0
while getopts "dmh" arg #选项后面的冒号表示该选项需要参数
do
        case $arg in
             d)
		DETAIL=1
                ;;
             m)
                echo "m"
                ;;
             h)
                echo "h"
                ;;
             ?)
            echo "unkonw argument"
        exit 1
        ;;
        esac
done

PHP_SLOW_LOG="/home/s/logs/php-slow.log"
NGINX_LOG=""
#   格式化输出
declare -a map
map=('\E[0m\c'
'\E[1;0m\c'
'\E[31m\c'
'\E[1;31m\c'
'\E[32m\c'
'\E[1;32m\c'
'\E[33m\c'
'\E[1;33m\c'
'\E[34m\c'
'\E[1;34m\c'
'\E[35m\c'
'\E[1;35m\c'
'\E[36m\c'
'\E[1;36m\c'
'\E[37m\c'
'\E[1;37m\c')

c_notify=13
c_error=3
c_sp=7
c_ori=5

cecho()
{
    local tmp=${map[$2]}
    message=$1
    color=${tmp:-$black}

    echo -e "$color"
    echo -e "$message"
    tput sgr0           # Reset to normal.
    echo -e "$black"
    return
}

headline()
{
    cecho "===========================================" $c_sp
    cecho "                 ""$1" $c_sp
    cecho "===========================================" $c_sp
}

export -f cecho

#内存占用
headline "load"
load=$(uptime | awk '{gsub(/ /, "");print $0}' | awk -F':' '{print $NF}' | awk -F',' '{print $1" "$2" "$3}')

if [ $DETAIL -gt 0 ];then
    cecho "origin load data: $load" $c_ori
fi

loadArr=(${load// / })

if [ `echo "${loadArr[0]} > 2" | bc` -eq 1 ];then
    cecho "1min load: "${loadArr[0]} $c_error
else
    cecho "1min load: "${loadArr[0]} $c_notify
fi


if [ `echo "${loadArr[1]} > 2" | bc` -eq 1 ];then
    cecho "5min load: "${loadArr[1]} $c_error
else
    cecho "5min load: "${loadArr[1]} $c_notify
fi

if [ `echo "${loadArr[2]} > 2" | bc` -eq 1 ];then
    cecho "15min load: "${loadArr[2]} $c_error
else
    cecho "15min load: "${loadArr[2]} $c_notify
fi

headline "memory"

freeData=$(free -m)
if [ $DETAIL -gt 0 ];then
    cecho "origin memory data:\n""$freeData" $c_ori
fi

line1=$(echo "$freeData" | sed -n '2p' | awk -F':' '{print $2}')
line1Arr=(${line1//\t/ })
systemLeft=$(echo ${line1Arr[2]} + ${line1Arr[4]} + ${line1Arr[5]} | bc)
if [ $systemLeft -gt 1024 ];then
    cecho "system memory left: "$systemLeft $c_notify
else
    cecho "system memory left: "$systemLeft $c_error
fi

line2=$(echo "$freeData" | sed -n '3p' | awk -F':' '{print $2}')
line2Arr=(${line2//\t/ })
if [ ${line2Arr[1]} -gt 1024 ];then
    cecho "application memory left: "${line2Arr[1]} $c_notify
else
    cecho "applicetion memory left: "${line2Arr[1]} $c_error
fi

line3=$(echo "$freeData" | sed -n '4p' | awk -F':' '{print $2}')
line3Arr=(${line3//\t/ })
if [ ${line3Arr[1]} -gt 0 ];then
    cecho "swap memory used: "${line3Arr[1]} $c_error
else
    cecho "swap memory used: "${line3Arr[1]} $c_notify
fi

# php slow log
# test if there are any recent slow log
headline "slow log"
stamp=$(tail -180 $PHP_SLOW_LOG | grep "(pool default)" | tail -1 | awk -F'pid' '{system("date -d\""$1"\" +%s")}')
((delta=$(date +%s) - $stamp))
if [  $delta -lt 7200 ];then
    cecho "slow log within 2 hours exists: ""$(tail -8 $PHP_SLOW_LOG)" $c_notify
else
    cecho "slow log within 2 hours NOT exists" $c_notify
fi

# iostat  sar mpstat 磁盘IO 网络IO 
cupInfo=$(mpstat)
cupInfo=$(echo $cupInfo | tail -1 | awk '{print $4,$5,$6,$7,$8,$9,$10,$11,$12}')
cupInfoMap=(${cupInfo// / })
#if [ $(echo ${cupInfo[0]} - 0.7 | bc > 0) -eq ]


# 

# nignx

# netstat time_wait
headline "time_wait"
timeWait=$(netstat -an | grep TIME_WAIT | wc -l)
if [ $timeWait -gt 15000 ];then
    cecho "TIME_WAIT: ""$timeWait" $c_error
else
    cecho "TIME_WAIT: ""$timeWait" $c_notify
fi
