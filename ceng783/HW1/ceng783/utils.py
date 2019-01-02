import cPickle
import gzip
import scipy.io as sio # you may use scipy for loading your data
from math import sqrt, ceil
import numpy as np
from random import randrange
import os

def load_mnist(path = '../data/mnist.pkl.gz'):
    "Load the MNIST dataset and return training, validation and testing data separately"
    f = gzip.open(path, 'rb')
    training_data, validation_data, test_data = cPickle.load(f)
    f.close()

    X_train, y_train = training_data[0], training_data[1]
    X_val, y_val = validation_data[0], validation_data[1]
    X_test, y_test = test_data[0], test_data[1]

    return X_train, y_train, X_val, y_val, X_test, y_test
    
    
def eval_numerical_gradient(f, x, verbose=False):
  """ 
  a naive implementation of numerical gradient of f at x 
  - f should be a function that takes a single argument
  - x is the point (numpy array) to evaluate the gradient at
  """

  fx = f(x) # evaluate function value at original point
  grad = np.zeros(x.shape)
  h = 0.00001

  # iterate over all indexes in x
  it = np.nditer(x, flags=['multi_index'], op_flags=['readwrite'])
  while not it.finished:

    # evaluate function at x+h
    ix = it.multi_index
    x[ix] += h # increment by h
    fxh = f(x) # evalute f(x + h)
    x[ix] -= h # restore to previous value (very important!)

    # compute the partial derivative
    grad[ix] = (fxh - fx) / h # the slope
    it.iternext() # step to next dimension

  return grad

def grad_check_sparse(f, x, analytic_grad, num_checks):
  """
  sample a few random elements and only return numerical
  in this dimensions.
  """
  h = 1e-5

  x.shape
  for i in xrange(num_checks):
    ix = tuple([randrange(m) for m in x.shape])

    x[ix] += h # increment by h
    fxph = f(x) # evaluate f(x + h)
    x[ix] -= 2 * h # increment by h
    fxmh = f(x) # evaluate f(x - h)
    x[ix] += h # reset

    grad_numerical = (fxph - fxmh) / (2 * h)
    grad_analytic = analytic_grad[ix]
    rel_error = abs(grad_numerical - grad_analytic) / (abs(grad_numerical) + abs(grad_analytic))
    print 'numerical: %f analytic: %f, relative error: %e' % (grad_numerical, grad_analytic, rel_error)
    
def visualize_grid(Xs, ubound=255.0, padding=1):
  """
  Reshape a 3D tensor of image data to a grid for easy visualization.

  Inputs:
  - Xs: Data of shape (D, D, H)
  - ubound: Output grid will have values scaled to the range [0, ubound]
  - padding: The number of blank pixels between elements of the grid
  """
  (D, D, H) = Xs.shape
  grid_size = int(ceil(sqrt(H)))
  grid_height = D * grid_size + padding * (grid_size - 1)
  grid_width = D * grid_size + padding * (grid_size - 1)
  grid = np.zeros((grid_height, grid_width))
  next_idx = 0
  y0, y1 = 0, D
  for y in xrange(grid_size):
    x0, x1 = 0, D
    for x in xrange(grid_size):
      if next_idx < H:
        img = Xs[:, :, next_idx]
        low, high = np.min(img), np.max(img)
        grid[y0:y1, x0:x1] = ubound * (img - low) / (high - low)
        # grid[y0:y1, x0:x1] = Xs[next_idx]
        next_idx += 1
      x0 += D + padding
      x1 += D + padding
    y0 += H + padding
    y1 += H + padding
  # grid_max = np.max(grid)
  # grid_min = np.min(grid)
  # grid = ubound * (grid - grid_min) / (grid_max - grid_min)
  return grid


import h5py
from scipy.signal import argrelextrema
import os

def split_data(PPG, ABG,split_size):
    splitted_PPG = []
    splitted_ABG = []
    tot_size = len(ABG)
    i = 0
    while i + split_size <= tot_size:
        splitted_PPG.append(PPG[i:i+split_size])
        splitted_ABG.append(ABG[i:i+split_size])
        i = i + split_size
    return splitted_PPG,splitted_ABG


def find_peaks(Arr):
    peaks = []
    for idx,l in enumerate(Arr):
        peakinds = argrelextrema(l, np.greater)[0]
        peakinds2 = argrelextrema(l, np.less)[0]
        extrema_max = l[peakinds]
        extrema_min = l[peakinds2]
        avg_max = extrema_max.sum()/(extrema_max.shape[0] + 1e-5)
        avg_min = extrema_min.sum()/(extrema_min.shape[0] + 1e-5)
        peaks.append((avg_max,avg_min))
    return peaks

def load_data(file_path, split_size):
    
    f = h5py.File(file_path,'r')
    data = f.get('Part_1').value
    X = []
    Y = []
    #ignore ECG
    for idx,item in enumerate(data):
        obj = item[0]
        PPG = f[obj].value[:, 0]
        ABG = f[obj].value[:, 1]
        PPG_list, ABG_list = split_data(PPG, ABG, split_size)
        peaks = find_peaks(ABG_list)
        for x,y in zip(PPG_list,peaks):
            X.append(x)
            Y.append(y)
    f.close()
    return np.array(X), np.array(Y)
def load_BP_dataset(filename):
        """Load your 'PPG to blood pressure' dataset"""
        # TODO: Fill this function so that your version of the data is loaded from a file into vectors
  
	# END OF YOUR CODE
        X, Y = load_data(filename, 182 * 4)

        return X, Y

