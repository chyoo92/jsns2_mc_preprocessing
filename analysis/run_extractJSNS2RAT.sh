#!/bin/bash
INPUTNUM=$1
INPUTRUN=$2
# INPUTFILE=/users/yewzzang/rat/RAT/rat-pac-jsns2-v2.0/mac/cylinder_root/cylinder$INPUTNUM.root

INPUTFILE=/store/hep/users/yewzzang/JSNS2/rat_mc_positron/positron_${INPUTRUN}MeV/positron_${INPUTRUN}MeV_${INPUTNUM}.root

# [ -z $1 ] && INPUTFILE=../simulate/output_1.root || INPUTFILE=$1

INPUTFILE=`readlink -f $INPUTFILE`
INPUTDIR=`dirname $INPUTFILE`
singularity run -B$INPUTDIR:$INPUTDIR /store/sw/singularity/jsns2/jsns2-jade0-20221212.sif <<EOF
make || exit
# ./extractMCEvent $INPUTFILE mc_data_1102/mcEvent_jsns2_${INPUTRUN}MeV_${INPUTNUM}.csv
# ./extractMCPMT $INPUTFILE mc_data_1102/mcPMT_jsns2_${INPUTRUN}MeV_${INPUTNUM}.csv
./extractMCEvent $INPUTFILE mc_data_1207/event_csv/mcEvent_pmtcharge_jsns2_${INPUTRUN}MeV_${INPUTNUM}.csv
./extractMCPMT $INPUTFILE mc_data_1207/pmt_csv/mcPMT_pmtcharge_jsns2_${INPUTRUN}MeV_${INPUTNUM}.csv
EOF



