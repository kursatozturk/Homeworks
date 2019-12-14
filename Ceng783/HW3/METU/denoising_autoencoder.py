import numpy as np
import matplotlib.pyplot as plt

identity = lambda x: x

class DenoisingAutoencoder(object):
    """
    Denoising autoencoder.
    """
    def sigmoid(self, x):
	#
	# TODO: implement sigmoid
	#
        return 1 / (1 + np.exp(-x))

    def sigmoid_deriv(self, x):
	#
	# TODO: implement sigmoid derivative
	#
        return x * (1 - x)

    def tanh_deriv(self, x):
        return 1 - np.power(x, 2)

    def ac_func(self, x, function_name = 'SIGMOID'):
        # Implement your activation function here
        fname_upper = function_name.upper()
        if fname_upper =='SIGMOID':
            return self.sigmoid(x)
        elif fname_upper == 'TANH':
            return np.tanh(x)
        else:
                raise fname_upper + " Not implemented Yet"

    def ac_func_deriv(self, x, function_name = 'SIGMOID'):
    	# Implement the derivative of your activation function here
        fname_upper = function_name.upper()
        if fname_upper == 'SIGMOID':
            return self.sigmoid_deriv(x)
        elif fname_upper == 'TANH':
            return self.tanh_deriv(x)

        else:
            raise fname_upper + " Not implemented Yet"
            
    def __init__(self, layer_units, weights=None):
        self.weights = weights
        self.layer_units = layer_units

    def init_weights(self, seed=0):
        """
        Initialize weights.

        layer_units: tuple stores the size of each layer.
        weights: structured weights.
        """

        """
        Initialize weights.

        layer_units: tuple stores the size of each layer.
        weights: structured weights.
        """

        # Note layer_units[2] = layer_units[0]
        layer_units = self.layer_units
        n_layers = len(layer_units)
        assert n_layers == 3

        np.random.seed(seed)

        # Initialize parameters randomly based on layer sizes
        r  = np.sqrt(6) / np.sqrt(layer_units[1] + layer_units[0])
        # We'll choose weights uniformly from the interval [-r, r)
        weights = [{} for i in range(n_layers - 1)]
        weights[0]['W'] = np.random.random((layer_units[0], layer_units[1])) * 2.0 * r - r
        weights[1]['W'] = np.random.random((layer_units[1], layer_units[2])) * 2.0 * r - r
        weights[0]['b'] = np.zeros(layer_units[1])
        weights[1]['b'] = np.zeros(layer_units[2])

        self.weights = weights

        return self.weights

    def predict(self, X_noisy, reg=3e-3, activation_function='sigmoid'):
        weights = self.weights

        # Weight parameters
        W0 = weights[0]['W']
        b0 = weights[0]['b']
        W1 = weights[1]['W']
        b1 = weights[1]['b']

        # TODO: Implement forward pass here. It should be the same as the forward pass that you implemented in the loss function
        
        h = self.ac_func(np.dot(X_noisy, W0) + b0, activation_function)
        scores = self.ac_func(np.dot(h, W1) + b1, activation_function)
        return scores

    def loss(self, X_noisy, X, reg=3e-3, activation_function='sigmoid'):
        weights = self.weights

        # Weighting parameters
        W0 = weights[0]['W']
        b0 = weights[0]['b']
        W1 = weights[1]['W']
        b1 = weights[1]['b']

        scores = None
        #############################################################################
        # TODO: Perform the forward pass, computing the  scores for the input. 	    #
        # Store the result in the scores variable, which should be an array of      #
        # shape (N, N).                                                             #
        #############################################################################
        
        h = self.ac_func(np.dot(X_noisy, W0) + b0, activation_function)
        scores = self.ac_func(np.dot(h, W1) + b1, activation_function)
        
        #############################################################################
        #                              END OF YOUR CODE                             #
        #############################################################################
        
        data_loss = 0.5 * np.mean(np.sum(np.square(X - scores), axis=1))

        l2_reg = 0.5 * reg * np.sum(np.sum(W * W) for W in [W0, W1])
        
        loss = data_loss + l2_reg

        #############################################################################
        # TODO: Compute the loss. This should include                               #
        #             (i) the data loss (square error loss),                        #
        #             (ii) L2 regularization for W1 and W2, and                     #
        # Store the result in the variable loss, which should be a scalar; i.e.,    #
        #   loss = 1/(2m) * sum_j sum_i (y^j_i - x^j_i) ^ 2  +  lambda/2 * ||W||^2, #
        # where m is the number of examples in your batch; j runs over examples in  #
        # the batch; i runs over your output neurons, and lambda is the     	    #
        # regularization weight.		    		                    		    #
        #############################################################################
        
        #############################################################################
        #                              END OF YOUR CODE                             #
        #############################################################################

        grads = {}
        dx = scores - X
        dscores = self.ac_func_deriv(scores, activation_function) * dx
        dh = self.ac_func_deriv(h, activation_function)
        
        dW1 = dscores.T.dot(h).T
        db1 = dscores.sum(axis = 0)
        
        tm = (dscores).dot(W1.T) * dh
        db0 = np.sum(tm, axis = 0)
        dW0 = X.T.dot(tm)
        #print X.shape, dscores.shape, W1.shape, dh.shape, dW0.shape

        dW1 /= dscores.shape[0]
        dW0 /= dscores.shape[0]
        dW1 += reg * W1
        dW0 += reg * W0

        grads = {
            1:{
                'W': dW1,
                'b': db1
            },
            0:{
                'W': dW0, 
                'b': db0,
            }
        }

        #############################################################################
        # TODO: Compute the backward pass, computing the derivatives of the weights #
        # and biases. Store the results in the grads dictionary. For example,       #
        # grads[1]['W'] should store the gradient on W1, and be a matrix of same    #
        # size. 								                                    #
        #############################################################################
            
        
        #############################################################################
        #                              END OF YOUR CODE                             #
        #############################################################################

        return loss, grads

    def train_with_SGD(self, X, noise=identity,
            learning_rate=1e-3, learning_rate_decay=0.95,
            reg=3e-3, num_iters=100,
            batchsize=128, momentum='classic', mu=0.9, verbose=False, 
            activation_function='sigmoid'):	

        num_train = X.shape[0]

        loss_history = []

        layer_units = self.layer_units
        n_layers = len(layer_units)
        velocity = [{} for i in range(n_layers - 1)]
        velocity[0]['W'] = np.zeros((layer_units[0], layer_units[1]))
        velocity[1]['W'] = np.zeros((layer_units[1], layer_units[2]))
        velocity[0]['b'] = np.zeros(layer_units[1])
        velocity[1]['b'] = np.zeros(layer_units[2])

        for it in xrange(num_iters):

            batch_indicies = np.random.choice(num_train, batchsize, replace = False)
            X_batch = X[batch_indicies]

            # Compute loss and gradients
            noisy_X_batch = noise(X_batch)
            loss, grads = self.loss(noisy_X_batch, X_batch, reg, activation_function=activation_function)
            loss_history.append(loss)

            #########################################################################
            # TODO: Use the gradients in the grads dictionary to update the         #
            # parameters of the network (stored in the dictionary self.params)      #
            # using gradient descent.                                               #
            #########################################################################
            dW0 = grads[0]['W']
            dW1 = grads[1]['W']
            db0 = grads[0]['b']
            db1 = grads[1]['b']

            velocity[0]['W'] = mu * velocity[0]['W'] + learning_rate * dW0
            velocity[1]['W'] = mu * velocity[1]['W'] + learning_rate * dW1
            velocity[0]['b'] = mu * velocity[0]['b'] + learning_rate * db0
            velocity[1]['b'] = mu * velocity[1]['b'] + learning_rate * db1 


            self.weights[0]['W'] -= velocity[0]['W']
            self.weights[1]['W'] -= velocity[1]['W']
            self.weights[0]['b'] -= velocity[0]['b']
            self.weights[1]['b'] -= velocity[1]['b']
            # You can start and test your implementation without momentum. After 
            # making sure that it works, you can add momentum


            #########################################################################
            #                             END OF YOUR CODE                          #
            #########################################################################

            if verbose and it % 10 == 0:
                print 'SGD: iteration %d / %d: loss %f' % (it, num_iters, loss)

            # Every 5 iterations.
            if it % 5 == 0:
                # Decay learning rate
                learning_rate *= learning_rate_decay

        return { 'loss_history': loss_history, }
