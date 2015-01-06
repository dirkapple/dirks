#!/bin/sh
color_echo()
{
    local black="\e[30m";
    local red="\e[31m";
    local green="\e[32m";
    local yellow="\e[33m";
    local blue="\e[34m";
    local purple="\e[35m";
    local cyan="\e[36m";
    local white="\e[37m";
    local reset="\e[0m";

    case $1 in
        'black') color=$black;;
        'red') color=$red;;
        'green') color=$green;;
        'yellow') color=$yellow;;
        'blue') color=$blue;;
        'purple') color=$purple;;
        'cyan') color=$cyan;;
        'white') color=$white;;
        *) color='';
    esac

    echo -ne "${color}$2${reset}";
}

color_echo 'green' "helo world"
