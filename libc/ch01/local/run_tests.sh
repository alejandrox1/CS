#!/bin/bash

set -e
GRE="\e[32m"                                                                    
END="\e[0m"

CFLAGS="-Wall -Werror -Wno-unused-variable"
IFLAGS="-I./../include"
LDFLAGS="-L./../lib -Wl,-rpath=./../lib"

tests=( "assert" )

for t in "${tests[@]}"; do
    EXEC="test_${t}"
    SRC="test_${t}.c"

    echo -e "${GRE}Testing ${t}...${END}\n"

    gcc ${CFLAGS} ${LDFLAGS} ${IFLAGS} -o ${EXEC} ${SRC} -lmyc && \
        ./${EXEC} && \
        rm ${EXEC}

    echo -e "\n"
done
