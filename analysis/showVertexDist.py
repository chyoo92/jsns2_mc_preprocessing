#!/usr/bin/env python

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

ds1 = pd.read_csv("mcEvent.csv")
ds2 = pd.read_csv("mcPMT.csv")
ds = pd.merge(ds1, ds2, how='inner')
del(ds1)
del(ds2)

rInner = 3.160/2
hInner = 2.398/2
rOuter = 3.6/2
hOuter = 3.680/2

ds.VertexX /= 1000
ds.VertexY /= 1000
ds.VertexZ /= 1000
ds['VertexR2'] = ds.VertexX**2+ds.VertexY**2

plt.hist2d(ds.VertexX, ds.VertexY, bins=(50,50), range=([-2, 2], [-2, 2]), cmap=plt.cm.Reds)
plt.plot(rInner*np.cos(np.linspace(0, 2*np.pi, 100)),
         rInner*np.sin(np.linspace(0, 2*np.pi, 100)), ':', color='k')
plt.show()

plt.hist2d(ds.VertexR2, ds.VertexZ, bins=(50,50), range=([0, 4], [-2, 2]), cmap=plt.cm.Reds)
plt.plot([0, 0, rInner**2, rInner**2, 0],
         [hInner, -hInner, -hInner, hInner, hInner], ':', color='k')
plt.plot([0, 0, rOuter**2, rOuter**2, 0],
         [hOuter, -hOuter, -hOuter, hOuter, hOuter], ':', color='k')
plt.show()

#for i in range(1, 97):
#    #plt.plot(ds['Charge%d' % i], ds['Time%d' % i], '.')
#for i in range(1, 97):
#  plt.plot(ds.VertexX, ds['Charge%d' % i], '.')
#  #plt.plot(ds.VertexX, ds['Time%d' % i], '.')
#  plt.show()
