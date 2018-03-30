#!/bin/bash

RED="\e[31m"
GRE="\e[32m"
END="\e[0m"

set -e

EXEC=simple-xor
KEY=ABCdef
PLAIN=message.txt
ENCR=encripted.txt
DECR=decripted.txt


echo -e "${GRE}Compiling...${END}"
gcc ${EXEC}.c -o $EXEC

echo -e "${GRE}Creating message...${END}"
echo "this is a test." > $PLAIN

echo -e "${GRE}Encripting...${END}"
./${EXEC} $KEY $PLAIN $ENCR

echo -e "${GRE}Decripting...${END}"
./${EXEC} $KEY $ENCR $DECR


echo -e "\n"
echo -e "${RED}Plain text:${END}" 
cat $PLAIN
echo
echo -e "${RED}Encripted msg:${END}"
cat $ENCR
echo
echo -e "${RED}Decripted msg:${END}" 
cat $DECR
echo

rm *.txt ${EXEC}
