import numpy
from keras.models import load_model


def get_new_ux(dt, y, ux):

  inputArray = numpy.array([[dt, y, ux]])
  #print ('\ntype(inputArray): ', type(inputArray))
  #print ('\ninputArray.shape: ', inputArray.shape) 
  
  filename = 'model_ux.h5'
  model = load_model(filename)

  prediction = model.predict(inputArray)
  
  return prediction


def get_new_uy(dt, y, uy):

  inputArray = numpy.array([[dt, y, uy]])
  
  filename = 'model_uy.h5'
  model = load_model(filename)

  prediction = model.predict(inputArray)
  
  return prediction


def get_new_f(dt, y, f):

  inputArray = numpy.array([[dt, y, f]])
  
  filename = 'model_f.h5'
  model = load_model(filename)

  prediction = model.predict(inputArray)
  
  return prediction

  
