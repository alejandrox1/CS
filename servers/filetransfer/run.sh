#!/bin/bash

rm -r testclient/* testserver/*.log testserver/*.out
make server && make client 
