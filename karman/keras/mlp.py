import numpy 
import matplotlib.pyplot as plt 
from pandas import read_csv 
import math 

from sklearn.preprocessing import StandardScaler
from sklearn.externals.joblib import dump 
from sklearn.externals.joblib import load

from keras.models import Sequential 
from keras.layers import Dense
from keras.layers import Dropout
from keras.models import load_model

# Number of points in the profile at first row of the .csv file
profile_points = 127 

# Iteration counter boundary.csv @first column (last row - first row + 1)
number_of_samples = 500

# Temporal stencil
look_back = 1 


# Convert an array of values into a dataset matrix 
def create_dataset(dataset, profile_points, inputs=3, outputs=1, look_back=1): 
  
  dataX, dataY = [], [] 
  for i in range(0,len(dataset)-look_back*profile_points): 
    dataX.append(dataset[i, 0:inputs])
    dataY.append(dataset[i + look_back*profile_points, inputs-outputs:inputs]) 
  
  return numpy.array(dataX), numpy.array(dataY)

# Constant random seed for reproducibility 
numpy.random.seed(7) 



targets = ['ux', 'uy', 'f']

for i, target in enumerate(targets): 

  # Load the dataset
  dataframe = read_csv('boundary.csv', usecols=[2,4,5+i], engine='python', skiprows=1)
  dataset = dataframe.values 
  dataset = dataset.astype('float32')
  filename = 'dataset_'+target+'.csv'
  #numpy.savetxt(filename, dataset, delimiter=',')
  #print('dataset.shape: ', dataset.shape) 

  # Split into train and test sets (order matters)
  train_samples = int(0.7*number_of_samples)
  #print('train_samples: ', train_samples)
  train_size = int(train_samples*(len(dataset)/number_of_samples)) 
  #print('profile points: ', int(len(dataset)/number_of_samples), ' of theoretical ', profile_points)
  #print('train_size: ', train_size)
  test_size = len(dataset) - train_size + look_back*profile_points
  #print('test_size: ', test_size)
  train, test = dataset[0:train_size,:], dataset[train_size-look_back*profile_points:len(dataset),:]
  #filename = 'train_'+target+'.csv'
  numpy.savetxt(filename, train, delimiter=',')
  #filename = 'test_'+target+'.csv'
  numpy.savetxt(filename, test, delimiter=',')

  # Reshape into X=t and Y=t+1 
  inputs = 3
  outputs = 1
  trainX, trainY = create_dataset(train, profile_points, inputs, outputs, look_back) 
  testX, testY = create_dataset(test, profile_points, inputs, outputs, look_back)
  filename = 'trainX_'+target+'.csv'
  numpy.savetxt(filename, trainX, delimiter=',')
  filename = 'trainY_'+target+'.csv'
  numpy.savetxt(filename, trainY, delimiter=',')
  filename = 'testX_'+target+'.csv'
  numpy.savetxt(filename, testX, delimiter=',')
  filename = 'testY_'+target+'.csv'
  numpy.savetxt(filename, testY, delimiter=',')

  trainX_size = len(trainX)
  testX_size = len(testX)
  #print('type(trainX): ', type(trainX)) 
  #print('trainX.shape: ', trainX.shape) 
  #print('trainX_size: ', trainX_size)
  #print('testX_size: ', testX_size)


  ## Scaler
  #scaler = StandardScaler().fit(trainX)
  #trainX = scaler.transform(trainX)



  ## TRAIN STAGE
  
  #print('\nTrain stage for ', target)

  ##filename = 'scaler_'+target+'.sav' 
  ##dump(scaler, filename)

  #batch_size = 4*profile_points
  #epochs = 200
  #kernel_initializer = 'normal' #'uniform'
  #loss = 'mean_squared_error' #'binary_crossentropy'
  #optimizer = 'adam'
  #activation = 'relu' 

  ## Create and fit Multilayer Perceptron model 
  #model = Sequential() 
  #model.add(Dense(profile_points, input_dim=inputs, kernel_initializer=kernel_initializer, activation=activation))
  ##model.add(Dropout(0.2))
  ##model.add(Dense(2*profile_points, kernel_initializer=kernel_initializer, activation=activation))
  ##model.add(Dropout(0.2))
  ##model.add(Dense(2*profile_points, kernel_initializer=kernel_initializer, activation=activation))
  #model.add(Dense(outputs)) 
  #model.compile(loss=loss, optimizer=optimizer) 
  #model.fit(trainX, trainY, epochs=epochs, batch_size=batch_size, verbose=2)

  ## Estimate model performance 
  #trainScore = model.evaluate(trainX, trainY, verbose=0) 
  #print('Train Score: %.2e MSE (%.2e RMSE)' % (trainScore, math.sqrt(trainScore))) 

  ## Creates a HDF5 file 'model.h5'
  #filename = 'model_'+target+'.h5'
  ##model.save(filename)
  #model.save(filename, include_optimizer=False) 
  ## Deletes the existing model
  ##del model  



  ## EVAL STAGE
  
  #print('\nEval stage for ', target)

  ### Load scaler
  ##filename = 'scaler_'+target+'.sav'
  ##loaded_scaler = load(filename)
  ##testX = loaded_scaler.transform(testX)

  ## Returns a compiled model identical to the saved one
  ##filename = 'model_'+target+'.h5'
  ##model = load_model(filename)

  #testScore = model.evaluate(testX, testY, verbose=0) 
  #print('Test Score: %.2e MSE (%.2e RMSE)' % (testScore, math.sqrt(testScore)))

  ## Generate predictions for training   
  #trainPredict = model.predict(trainX)   
  #testPredict = model.predict(testX) 
  #filename = 'trainPredict_'+target+'.csv'
  ##numpy.savetxt(filename, trainPredict, delimiter=',')
  #filename = 'testPredict_'+target+'.csv'
  ##numpy.savetxt(filename, testPredict, delimiter=',')



  ## PRINT
  
  #print('\nPrint stage for ', target)
  
  ## From 0 to 126
  ## Test with 0, 64 and 126
  #point_y = 64

  #target_predict_plot = 0
  #target_dataset_plot = 2
   

  #datasetplot = dataset[point_y:len(dataset)-look_back*profile_points:profile_points,target_dataset_plot]
  #filename = 'datasetplot_'+target+'.csv'
  #numpy.savetxt(filename, datasetplot, delimiter=',')
  #print('datasetplot: ', datasetplot.shape)

  #trainPredictPlot = numpy.empty_like(datasetplot) 
  #trainPredictPlot[:] = numpy.nan 
  #print('trainPredict: ', trainPredict.shape)
  #print('trainPredictPlot before: ', trainPredictPlot.shape)
  #trainPredictPlot = trainPredict[point_y:len(trainPredict):profile_points,target_predict_plot]
  #print('trainPredictPlot after: ', trainPredictPlot.shape)
  #filename = 'trainPredictPlot_'+target+'.csv'
  #numpy.savetxt(filename, trainPredictPlot, delimiter=',')

  #testPredictPlot = numpy.empty_like(datasetplot) 
  #testPredictPlot[:] = numpy.nan 
  #print('testPredict: ', testPredict.shape)
  #print('testPredictPlot before: ', testPredictPlot.shape)
  #testPredictPlot[len(trainPredictPlot):] = testPredict[point_y:len(testPredict):profile_points,target_predict_plot]
  #print('testPredictPlot after: ', testPredictPlot.shape)
  #filename = 'testPredictPlot_'+target+'.csv'
  #numpy.savetxt(filename, testPredictPlot, delimiter=',')

  ## Plot baseline and predictions 
  #plt.plot(datasetplot)
  #plt.plot(trainPredictPlot) 
  #plt.plot(testPredictPlot) 
  #plt.show()





