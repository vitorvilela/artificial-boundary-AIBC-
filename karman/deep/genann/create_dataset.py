import numpy 
from pandas import read_csv 
import math 

# Number of points in the profile, at first row of the dataset/boundary.csv file
profile_points = 127 

# Iteration counter 'i'. At boundary.csv, first column and (last row value - first row value + 1)
number_of_samples = 500

# Temporal stencil
look_back = 1 

# Constant random seed for reproducibility 
numpy.random.seed(7) 


# Convert an array of values into a dataset matrix 
def create_dataset(dataset, profile_points, inputs=3, outputs=1, look_back=1): 
  
  dataX, dataY = [], [] 
  for i in range(0,len(dataset)-look_back*profile_points): 
    dataX.append(dataset[i, 0:inputs])
    dataY.append(dataset[i + look_back*profile_points, inputs-outputs:inputs]) 
  
  return numpy.array(dataX), numpy.array(dataY)


targets = ['ux', 'uy', 'f']

for i, target in enumerate(targets): 

  # Load the dataset
  dataframe = read_csv('./dataset/boundary.csv', usecols=[2,4,5+i], engine='python', skiprows=1)
  dataset = dataframe.values 
  dataset = dataset.astype('float64')
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
  #numpy.savetxt(filename, train, delimiter=',')
  #filename = 'test_'+target+'.csv'
  #numpy.savetxt(filename, test, delimiter=',')

  # Reshape into X=t and Y=t+1 
  inputs = 3
  outputs = 1
  trainX, trainY = create_dataset(train, profile_points, inputs, outputs, look_back) 
  testX, testY = create_dataset(test, profile_points, inputs, outputs, look_back)
  filename = './dataset/trainX_'+target+'.csv'
  numpy.savetxt(filename, trainX, delimiter=',')
  filename = './dataset/trainY_'+target+'.csv'
  numpy.savetxt(filename, trainY, delimiter=',')
  filename = './dataset/testX_'+target+'.csv'
  numpy.savetxt(filename, testX, delimiter=',')
  filename = './dataset/testY_'+target+'.csv'
  numpy.savetxt(filename, testY, delimiter=',')

  #trainX_size = len(trainX)
  #testX_size = len(testX)
  #print('type(trainX): ', type(trainX)) 
  #print('trainX.shape: ', trainX.shape) 
  #print('trainX_size: ', trainX_size)
  #print('testX_size: ', testX_size)
