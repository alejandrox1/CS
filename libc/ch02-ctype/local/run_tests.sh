#!/bin/bash

set -e
GRE="\e[32m"                                                                    
END="\e[0m"

if [ ! -f lib/libmyc.so ]; then
    make && make clean
fi

CFLAGS="-Wall -Werror -Wno-unused-variable"
IFLAGS="-Iinclude"
LDFLAGS="-Llib -Wl,-rpath=lib"

tests=$(find . -name "test_*" -a -name "*.c")
for t in ${tests}; do
    EXEC="${t::-2}"
    echo -e "${GRE}Testing ${t}...${END}\n"

    gcc ${CFLAGS} ${LDFLAGS} ${IFLAGS} -o ${EXEC} ${t} -lmyc && \
        ./${EXEC} && \
        rm ${EXEC}

    echo -e "\n"
done
