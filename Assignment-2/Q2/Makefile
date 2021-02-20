# Usage:
# make 		 # compile all files 
# make plot  # create plots
# make clean # cleanup

.PHONY = plot clean

CC = gcc
PYTHON = python3

LINKERFLAG = -lm

SRCS := $(wildcard *.c)
BINS := main

${BINS}: ${SRCS}
	@echo "Building..."
	${CC} -o $@ $^ ${LINKERFLAG}
	@mkdir data

plot: plot.py
	@mkdir plots
	@echo "Plotting..."
	${PYTHON} $<

clean:
	@echo "Cleaning..."
	rm -rvf ${BINS} data plots