#!/bin/sh

[ -z "$1" ] && echo "$0 [angle]" && exit

[ `pgrep ToyViewer |wc -l` -ne 0 ] && echo "kill ToyViewer" && pkill -9 ToyViewer
[ ! -f "write" ] && echo "build write" && gcc -o write write.c
[ ! -f "sample.pnm" ] && echo "make sample.pnm" && ./write
gcc -o rotate rotate.c image.c matrix.c && ./write && ./rotate $1 && open -a ToyViewer sample_rev.pnm
