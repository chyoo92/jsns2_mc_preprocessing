#!/bin/bash

# bash run_extractJSNS2RAT.sh 10 10

for i in {10..150..10}
do
for j in {0..100..1}
do
bash run_extractJSNS2RAT.sh ${j} ${i}
done
done