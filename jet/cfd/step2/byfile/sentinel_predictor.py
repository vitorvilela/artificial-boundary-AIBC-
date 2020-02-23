from __future__ import absolute_import, division, print_function, unicode_literals

import os
import time
import sys
import numpy as np

import tensorflow as tf
from tensorflow import keras

#print(tf.version.VERSION)
#print(tf.keras.__version__)

target = sys.argv[1]

print('\nSENTINEL PREDICTOR FOR {}\n'.format(target))

input_filename = './input/{}_batch_prediction_inputs.csv'.format(target)
output_filename = './output/{}_batch_prediction_output.csv'.format(target)

control_filename = './sentinel_predictor_{}.end'.format(target)

model_filename = './models/{}_model.h5'.format(target)
model = keras.models.load_model(model_filename)

control = True

while control:
  
  # Check if input files are available on every 0.5 second
  time.sleep(0.5) 
  print('still alive {}'.format(target))
  
  if os.path.isfile(input_filename):
    
    print('\n...a new {} is available\n'.format(input_filename))
    
    model_input = np.genfromtxt(input_filename, skip_header=1, delimiter=',')

    #model_input[:,0] = model_input[:,0]*8.72058855e-07 + 0.03906227 # dt
    #model_input[:,1] = model_input[:,1]*1.79006863e+00 + 0. # y
    #if target == 'ux':
    #  model_input[:,2] = model_input[:,2]*4.32956065e-02 + 0.01575622
    #elif target == 'uy':
    #  model_input[:,2] = model_input[:,2]*3.01796625e-02 + 0.00014773
    #elif target == 'f':
    #  model_input[:,2] = model_input[:,2]*2.49605243e-01 + 0.08669415 
        
    print('start prediction {}'.format(target))        
    model_predictions = model.predict(model_input)
    print('end prediction {}'.format(target))
    
    np.savetxt(output_filename, model_predictions)
    
    os.remove(input_filename)
    
  if os.path.isfile(control_filename):
    control = False
    