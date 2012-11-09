#!/bin/sh

[ -z "$1" ] && exit

gcc -o rotate rotate.c matrix.c && ./write && ./rotate $1 && open -a ToyViewer sample_rev.pnm
