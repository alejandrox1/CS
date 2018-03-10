#!/bin/bash

make clean && \
	make original && \
	echo && \
	sudo ./contained -m $(pwd)/jail -u 0 -c /bin/sh
