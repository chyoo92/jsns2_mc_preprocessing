#!/bin/bash
# INPUTNUM=$1
# INPUTFILE=/users/yewzzang/rat/RAT/rat-pac-jsns2-v2.0/mac/cylinder_root/cylinder$INPUTNUM.root
INPUTFILE=/users/yewzzang/rat/RAT/rat-pac-jsns2-v2.0/mac/jsns2/50k_positron.root

# [ -z $1 ] && INPUTFILE=../simulate/output_1.root || INPUTFILE=$1

INPUTFILE=`readlink -f $INPUTFILE`
INPUTDIR=`dirname $INPUTFILE`
singularity run -B$INPUTDIR:$INPUTDIR /store/sw/singularity/jsns2/jsns2-jade0-20221212.sif <<EOF
make || exit
./extractMCEvent $INPUTFILE mcEvent_jsns2_default50k.csv
./extractMCPMT $INPUTFILE mcPMT_jsns2_default50k.csv
EOF
