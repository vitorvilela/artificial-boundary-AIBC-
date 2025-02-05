# CFD to TF temporal dataset

import numpy as np
import matplotlib.pyplot as plt 
from pandas import read_csv 
import math 

dataset_path = './serial-n128/boundary.csv'
print_former = False
print_intermediate = False
print_data_info = True

# Number of points in profile. See the first row of the boundary.csv
profile_points = 127 

# Number of samples. In the first column of the boundary.csv: (last_i - first_i + 1)
first_i = 90
last_i = 543
number_of_samples = last_i-first_i+1

# Train proportion
train_proportion = 0.7

# Temporal stencil
look_back = 1 

# Number of inputs and outputs
inputs = 3
outputs = 1



# Convert an array of values into a dataset matrix 
def create_dataset(dataset, profile_points, inputs=3, outputs=1, look_back=1): 
  
  dataX, dataY = [], [] 
  for i in range(0,len(dataset)-look_back*profile_points): 
    dataX.append(dataset[i, 0:inputs])
    dataY.append(dataset[i + look_back*profile_points, inputs-outputs:inputs]) 
  
  return numpy.array(dataX), numpy.array(dataY)


# Constant random seed for reproducibility 
numpy.random.seed(7) 

# Targets
targets = ['ux', 'uy', 'f']

for i, target in enumerate(targets): 

  # Load the dataset
  dataframe = read_csv(dataset_path, usecols=[2,4,5+i], engine='python', skiprows=1)
  dataset = dataframe.values 
  dataset = dataset.astype('float32')
    
  if print_former:
    filename = 'dataset_{}.csv'.format(target)
    numpy.savetxt(filename, dataset, delimiter=',')
    print('dataset.shape: ', dataset.shape) 


  # Split into train and test datasets. Order matters!
  train_samples = int(train_proportion*number_of_samples)  
  train_size = int(train_samples*(len(dataset)/number_of_samples)) 
  test_size = len(dataset) - train_size + look_back*profile_points
  train, test = dataset[0:train_size,:], dataset[train_size-look_back*profile_points:len(dataset),:]
   
  if print_intermediate:
    print('train_samples: ', train_samples)
    print('profile points: ', int(len(dataset)/number_of_samples), ' of theoretical ', profile_points)
    print('train_size: ', train_size)
    print('test_size: ', test_size)    
    filename = 'intermediate_train_{}.csv'.format(target)
    numpy.savetxt(filename, train, delimiter=',')
    filename = 'intermediate_test_{}.csv'.format(target)
    numpy.savetxt(filename, test, delimiter=',')
    print(filename)


  # Reshape into X=t and Y=t+1 
  trainX, trainY = create_dataset(train, profile_points, inputs, outputs, look_back) 
  testX, testY = create_dataset(test, profile_points, inputs, outputs, look_back)
  filename = 'trainX_{}.csv'.format(target)
  numpy.savetxt(filename, trainX, delimiter=',')
  filename = 'trainY_{}.csv'.format(target)
  numpy.savetxt(filename, trainY, delimiter=',')
  filename = 'testX_{}.csv'.format(target)
  numpy.savetxt(filename, testX, delimiter=',')
  filename = 'trainY_{}.csv'.format(target)
  numpy.savetxt(filename, testY, delimiter=',')
  
  if print_data_info:    
    trainX_size = len(trainX)
    testX_size = len(testX)
    trainY_size = len(trainY)
    testY_size = len(testY)
    print('\n')
    print('type(trainX): ', type(trainX)) 
    print('trainX.shape: ', trainX.shape) 
    print('trainX_size: ', trainX_size)
    print('testX_size: ', testX_size)
    print('type(trainY): ', type(trainY)) 
    print('trainY.shape: ', trainY.shape) 
    print('trainY_size: ', trainY_size)
    print('testY_size: ', testY_size)
    
