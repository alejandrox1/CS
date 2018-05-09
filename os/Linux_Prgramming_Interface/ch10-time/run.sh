#!/bin/bash

BLU="\e[34;1m"
GRE="\e[32m"
NOC="\e[0m"


echo -e "${BLU}Testing strtime...${NOC}" && \
    make clean && make strtime && \
    echo -e "${GRE}\"9:39:46pm 1 Feb 2011\" \"%I:%M:%S%p %d %b %Y\"${NOC}\n" && \
    ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y" && \
    echo && \
    echo -e "${GRE}\"9:39:46pm 1 Feb 2011\" \"%I:%M:%S%p %d %b %Y\" \"%F %T\"${NOC}" && \
    ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y" "%F %T"
