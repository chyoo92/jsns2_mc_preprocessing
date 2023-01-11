# For make mc .h5 file data and testing...

## um.............


#### Extract event information from MC data. 
##### Q,True Vertex, etc.... (made by JH.Goh)
- extractMCEvent
- extractMCEvent.cc
- extractMCPMT
- extractMCPMT.cc
- showVertexDist.py
####

- run_extractJSNS2RAT.sh : script use extractMCEvent.cc & extractMCPMT.cc (need lab singularity file)
- loop_run_extractJSNS2RAT.sh : loop run_extractJSNS2RAT.sh script

####
- csv_to_h5file.py : after run run_extractJSNS2RAT.sh file. csv file -> h5 file (for use in deep learning)

#### data confirm...
- jsns2.ipynb : visualization event position
- lab_jsns2_diff : PMT charge diff check



##### old file (used for KPS)
- cylinder_plot.ipynb : cylinder detector (RAT example)
- sphere_plot.ipynb : sphere detector (RAT example)
