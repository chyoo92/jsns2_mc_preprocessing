import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import uproot
import h5py
energy = [20,30,40,50,60,70,80,90,100,110,120,130,140,150]
for k in energy:

    output_name = "mc_data_1102/combined_"+str(k)+"MeV.h5"
    out_vertex  = np.array([]).reshape(0,3)
    out_pmtQ = np.array([]).reshape(0,120)
    out_jade_vertex = np.array([]).reshape(0,3)
    for i in range(101):

        ds_E = pd.read_csv("mc_data_1102/mcEvent_jsns2_"+str(k)+"MeV_"+str(i)+".csv")
        ds_P = pd.read_csv("mc_data_1102/mcPMT_jsns2_"+str(k)+"MeV_"+str(i)+".csv")
        vertex = np.array(ds_E.iloc[:,-4:-1])
        pmtQ = np.array(ds_P.iloc[:,3:123])

        path = "/store/hep/users/yewzzang/JSNS2/rat_mc_positron/positron_"+str(k)+"MeV/jade/reco/Reco_positron_"+str(k)+"MeV_"+str(i)+".root"
        f = uproot.open(path)

        test = f['recoTriggerTree']['recoTrigger/events/events.vertexReco'].array()

        for j in range(len(test)):

            if (np.array(test[j]).shape[0] == 1) & (np.abs(vertex[j][2])<1200):
                out_jade_vertex = np.concatenate((out_jade_vertex,np.array([np.array(test[j])[0][0],np.array(test[j])[0][1],np.array(test[j])[0][2]]).reshape(1,3)),axis = 0)
                out_vertex = np.concatenate((out_vertex,vertex[j].reshape(1,3)),axis = 0)
                out_pmtQ = np.concatenate((out_pmtQ,pmtQ[j].reshape(1,120)),axis = 0)


    kwargs = {'dtype':'f4', 'compression':'lzf'}
    #     with h5py.File(output_path, 'w', libver='latest', swmr=True) as fout:

    with h5py.File(output_name, 'w', libver='latest', swmr=True) as fout:
        g = fout.create_group('events')

        g.create_dataset('pmtQ', data=out_pmtQ, chunks=(1,120),**kwargs)
        g.create_dataset('jade_vertex', data=out_jade_vertex, chunks=(1,3),**kwargs)
        g.create_dataset('vertex', data=out_vertex, chunks=(1,3),**kwargs)