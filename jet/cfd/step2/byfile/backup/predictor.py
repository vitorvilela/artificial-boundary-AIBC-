from __future__ import absolute_import, division, print_function, unicode_literals

import os
import sys
import numpy as np

import tensorflow as tf
from tensorflow import keras

#print(tf.version.VERSION)
#print(tf.keras.__version__)

target = sys.argv[1]

input_filename = './input/{}_batch_prediction_inputs.csv'.format(target)
model_input = np.genfromtxt(input_filename, skip_header=1, delimiter=',')

#model_input[:,0] = model_input[:,0]*8.72058855e-07 + 0.03906227 # dt
#model_input[:,1] = model_input[:,1]*1.79006863e+00 + 0. # y

#if target == 'ux':
#  model_input[:,2] = model_input[:,2]*4.32956065e-02 + 0.01575622
#elif target == 'uy':
#  model_input[:,2] = model_input[:,2]*3.01796625e-02 + 0.00014773
#elif target == 'f':
#  model_input[:,2] = model_input[:,2]*2.49605243e-01 + 0.08669415 
    
model_path = './models/{}_model.h5'.format(target)
model = keras.models.load_model(model_path)
model_predictions = model.predict(model_input)

output_filename = './output/{}_batch_prediction_output.csv'.format(target)
np.savetxt(output_filename, model_predictions)