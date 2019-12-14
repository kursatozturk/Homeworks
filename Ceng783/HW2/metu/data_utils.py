import scipy.io as sio # you may use scipy for loading your data
import cPickle as pickle # or Pickle library
import numpy as np
import os


def load_dataset(filename):
 	"""Load your 'PPG to blood pressure' dataset"""
	# TODO: Fill this function so that your version of the data is loaded from a file into vectors

	return X, Y



if __name__ == '__main__':
	# TODO: You can fill in the following part to test your function(s)/dataset from the command line
	filename='...'
	X, Y = load_dataset(filename)
