#!/bin/bash
#
# show_time:
#   If you run into trouble while trying to run `show_time` with an error
#   saying:  
#       [2 No such file or directory] setlocale
#   Then you may want to go and `sudo vi /etc/locale.gen` and uncomment
#   whatever locales you want to have available.
#   After this, do `sudo locale-gen` and you should be set to go.
#
BLU="\e[34;1m"
GRE="\e[32m"
NOC="\e[0m"


# 1. Run with the set from env.
# 2. Run with German locale.
# 3. Run with German and Italian locales - LC_TIME has precedence over LANG.
# 4. Run with French and US locales - LC_ALL has precendece over LC_TIME.
echo -e "${BLU}Testing show_time...${NOC}" && \
    make clean && make show_time && \
    echo -e "${GRE}./show_time${NOC}" && \
    ./show_time  && \
    echo && \
    echo -e "${GRE}LANG=de_DE ./show_time${NOC}" && \
    LANG=de_DE ./show_time && \
    echo && \
    echo -e "${GRE}LANG=de_DE LC_TIME=it_IT ./show_time${NOC}" && \
    LANG=de_DE LC_TIME=it_IT ./show_time && \
    echo && \
    echo -e "${GRE}LC_ALL=fr_FR LC_TIME=en_US ./show_time${NOC}" && \
    LC_ALL=fr_FR LC_TIME=en_US ./show_time


echo -e "${BLU}Testing strtime...${NOC}" && \
    make clean && make strtime && \
    echo -e "${GRE}\"9:39:46pm 1 Feb 2011\" \"%I:%M:%S%p %d %b %Y\"${NOC}\n" && \
    ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y" && \
    echo && \
    echo -e "${GRE}\"9:39:46pm 1 Feb 2011\" \"%I:%M:%S%p %d %b %Y\" \"%F %T\"${NOC}" && \
    ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y" "%F %T"

echo -e "${BLU}Testing process_time...${NOC}" && \
    make clean && make process_time && \
    echo -e "${GRE}./process_time 10000000${NOC}" && \
    ./process_time 10000000

make clean
