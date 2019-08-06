from root_numpy import root2array, tree2array, list_structures
from root_numpy import testdata
import numpy as np
import pandas

import keras
from keras.models import Sequential
import matplotlib.pyplot as plt
from keras.layers import Dense, Dropout, Activation, Flatten, Merge
from keras.layers import Conv2D, MaxPooling2D

from sklearn.metrics import roc_curve, auc

filenameBkg = 'uhh2.AnalysisModuleRunner.MC.TTbar.root'
filenameSig = 'uhh2.AnalysisModuleRunner.MC.SingleTop_tWch.root'

# Convert a TTree in a ROOT file into a NumPy structured array + pandas Dataframe, with which you can create an array to be given to Keras
arrBkg = pandas.DataFrame(root2array(filenameBkg, treename='AnalysisTree',branches=["tmva_ptbalance","tmva_deltaphi_bottomlepton","tmva_ht_jets","tmva_deltaphi_bottomtoptag"]))
arrSig = pandas.DataFrame(root2array(filenameSig, treename='AnalysisTree',branches=["tmva_ptbalance","tmva_deltaphi_bottomlepton","tmva_ht_jets","tmva_deltaphi_bottomtoptag"]))
# one needs probably a flat tree, to access information

#print arrBkg

#save the numpy format
outfileBkg = 'outBkg.npy'
np.save(outfileBkg,arrBkg) #save as numpy

#arr.to_csv('out.csv') #save as csv

outfileSig = 'outSig.npy'
np.save(outfileSig,arrSig) #save as numpy

#create array for Keras implementation
Arr_Bkg = np.load(outfileBkg)
Arr_Sig = np.load(outfileSig)

Values_array = np.concatenate((Arr_Bkg,Arr_Sig),axis=0)

#print new_arr.shape #shape is number of events x number of variables
#print new_arr[:][1] #these are the separate elements of the array 

label_bkg = np.zeros(Arr_Bkg.shape[0])
label_sig = np.ones(Arr_Sig.shape[0])

Labels_array = np.concatenate((label_bkg,label_sig),axis=0)

print 'Variables setup'

#trying a NN implementation

#transforming arrays in float
Labels_array = Labels_array.astype(np.float)
Values_array = Values_array.astype(np.float)

print Values_array.shape
print Labels_array

print 'Defining the model'

model = Sequential()
model.add(Dense(60, input_dim=4, activation='sigmoid'))
model.add(Dense(80, activation='tanh'))
model.add(Dense(80, activation='tanh'))
model.add(Dense(10, activation='tanh'))
model.add(Dense(1, activation='sigmoid'))

print 'Compiling the model'

# Compile model                                                                                                                                                                                            
model.compile(loss='mean_squared_error', optimizer='rmsprop', metrics=['accuracy'])
# Fit the model (this is the training!!!)                                                                                                                                                                  
history = model.fit(Values_array, Labels_array, epochs=10, batch_size=1024)

print(model.summary())

#plot the loss as a function of the epoch                                                                                                                                                                   
# summarize history for accuracy                                                                                                                                                                           
plt.plot(history.history['acc'])
plt.title('model accuracy')
plt.ylabel('accuracy')
plt.xlabel('epoch')
plt.legend(['train', 'test'], loc='upper left')
plt.show()
# summarize history for loss                                                                                                                                                                               
plt.plot(history.history['loss'])
plt.title('model loss')
plt.ylabel('loss')
plt.xlabel('epoch')
plt.legend(['train', 'test'], loc='upper left')
plt.show()
