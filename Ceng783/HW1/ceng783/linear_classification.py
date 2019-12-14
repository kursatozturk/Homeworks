import numpy as np
import matplotlib.pyplot as plt
from math import exp #for softmax

class LinearClassifier(object):
  """
  A linear classifier with hinge loss or softmax loss.
  """

  def __init__(self, input_size, output_size, std=1e-4):
    """
    Initialize the model. Weights are initialized to small random values and
    biases are initialized to zero. Weights and biases are stored in the
    variable self.params, which is a dictionary with the following keys:

    W: Layer weights; has shape (C, D)  -- bias is included here.   

    Inputs:
    - input_size: The dimension D of the input data.
    - output_size: The number of classes C.
    """
    self.params = {}
    self.params['W'] = std * np.random.randn(output_size, input_size) # This was is easier; we get rid of taking the transpose in multiplications    
  
  def naive_hinge_loss(self, X, y=None, W=None, reg=0.0, delta=1):
    """
    Compute the loss and gradients.

    Inputs:
    - X: Input data of shape (N, D). Each X[i] is a training sample.
    - y: Vector of training labels. y[i] is the label for X[i], and each y[i] is
      an integer in the range 0 <= y[i] < C. This parameter is optional; if it
      is not passed then we only return scores, and if it is passed then we
      instead return the loss and gradients.
    - reg: Regularization strength.
    - loss_type: 'hinge' for hinge loss, 'xentropy' for cross-entropy loss.

    Returns:
    If y is None, return a matrix scores of shape (N, C) where scores[i, c] is
    the score for class c on input X[i].

    If y is not None, instead return a tuple of:
    - loss: Loss (data loss and regularization loss) for this batch of training
      samples.
    - grads: Dictionary mapping parameter names to gradients of those parameters
      with respect to the loss function; has the same keys as self.params.
    """
    # Unpack variables from the params dictionary
    if W is None: W = self.params['W']
    N, D = X.shape
    
    scores = None
    loss = 0.0
    
    dW = np.zeros(W.shape) # initialize the gradient as zero
    # compute the loss and the gradient
    # W: Layer weights has shape (C, D)
    num_classes = W.shape[0]
    num_train = X.shape[0]
    for i in xrange(num_train):
      X_i = X[i] # ith training instance     
      scores = W.dot(X_i)   # W . X_i  (bias is included in W & X
      correct_class_score = scores[y[i]] # score for the correct label
      for j in xrange(num_classes):
        if j == y[i]:
          continue
        margin = scores[j] - correct_class_score + delta 
        if margin > 0:
          dW[j, :] += X_i
          dW[y[i], :] -= X_i
          loss += margin

    # Right now the loss is a sum over all training examples, but we want it
    # to be an average instead so we divide by num_train.
    loss /= num_train
    dW /= num_train

    # Add regularization to the loss.
    loss += 0.5 * reg * np.sum(W * W)

    #############################################################################
    # TODO: Compute the gradient of the loss function and store it dW.          #
    # Rather that first computing the loss and then computing the derivative,   #
    # it may be simpler to compute the derivative at the same time that the     #
    # loss is being computed. As a result you may need to modify some of the    #
    # code above to compute the gradient.                                       #
    #############################################################################
    
    
    ###########################################################################
    #                              END OF YOUR CODE                           #
    ###########################################################################
  

    return loss, dW

  def vectorized_hinge_loss(self, X, y=None, W=None, reg=0.0, delta=1):
    """
    Compute the loss and gradients.

    Inputs:
    - X: Input data of shape (N, D). Each X[i] is a training sample.
    - y: Vector of training labels. y[i] is the label for X[i], and each y[i] is
      an integer in the range 0 <= y[i] < C. This parameter is optional; if it
      is not passed then we only return scores, and if it is passed then we
      instead return the loss and gradients.
    - reg: Regularization strength.
    - delta: margin in hinge-loss definition.

    Returns:
    If y is None, return a matrix scores of shape (N, C) where scores[i, c] is
    the score for class c on input X[i].

    If y is not None, instead return a tuple of:
    - loss: Loss (data loss and regularization loss) for this batch of training
      samples.
    - grads: Dictionary mapping parameter names to gradients of those parameters
      with respect to the loss function; has the same keys as self.params.
    """
    # Unpack variables from the params dictionary
    if W is None: W = self.params['W'] # has shape C x D
    N, D = X.shape  

    # Compute the forward pass
    #############################################################################
    # TODO: Perform the forward pass, computing the class scores for the input. #
    # Store the result in the scores variable, which should be an array of      #
    # shape (N, C).                                                             #
    #############################################################################

    
    number_of_training = N # alias for N

    scores = W.dot(X.T).T

    #############################################################################
    #                              END OF YOUR CODE                             #
    #############################################################################
    
    # If the targets are not given then jump out, we're done
    if y is None:
      return scores

    # Compute the loss
    #############################################################################
    # TODO: Finish the forward pass, and compute the loss. This should include  #
    # both the data loss and L2 regularization for W. Store the result  	      #
    # in the variable loss, which should be a scalar. Use the Softmax           #
    # classifier loss. So that your results match ours, multiply the            #
    # regularization loss by 0.5                                                #
    #############################################################################
    scores = np.exp(scores.T)
    scores_total = np.sum(scores, axis = 0)
    scores = (scores / scores_total).T
    
    margins = np.maximum(0, scores.T - scores[np.arange(number_of_training), y] + delta)
    margins[y, np.arange(number_of_training)] = 0

    loss = np.sum(margins)
    loss /= number_of_training  
    loss += np.sum(W * W) * reg * 0.5

    #############################################################################
    #                              END OF YOUR CODE                             #
    #############################################################################
   
    #############################################################################
    # TODO: Compute the backward pass, computing the derivatives of the weights.#
    # Store the results in the `grad' variable, which should be a matrix that   #
    # has the same size as 'W' 			                                    				#
    #############################################################################
    tmp = (margins > 0) * 1
    tmp[y, np.arange(number_of_training)] -= tmp.sum(axis = 0)

    grad = tmp.dot(X)
    grad /= number_of_training
    grad += reg * W
    #############################################################################
    #                              END OF YOUR CODE                             #
    #############################################################################

    return loss, grad
    
  def vectorized_xentropy_loss(self, X, y=None, W=None, reg=0.0):
    """
    Compute the loss and gradients.

    Inputs:
    - X: Input data of shape (N, D). Each X[i] is a training sample.
    - y: Vector of training labels. y[i] is the label for X[i], and each y[i] is
      an integer in the range 0 <= y[i] < C. This parameter is optional; if it
      is not passed then we only return scores, and if it is passed then we
      instead return the loss and gradients.
    - reg: Regularization strength.

    Returns:
    If y is None, return a matrix scores of shape (N, C) where scores[i, c] is
    the score for class c on input X[i].

    If y is not None, instead return a tuple of:
    - loss: Loss (data loss and regularization loss) for this batch of training
      samples.
    - grads: Dictionary mapping parameter names to gradients of those parameters
      with respect to the loss function; has the same keys as self.params.
    """
    # Unpack variables from the params dictionary
    if W is None: W = self.params['W'] # has shape C x D
    N, D = X.shape
    
    # Compute the forward pass
    scores = None
    #############################################################################
    # TODO: Perform the forward pass, computing the class scores for the input. #
    # Store the result in the scores variable, which should be an array of      #
    # shape (N, C).                                                             #
    #############################################################################
    number_of_training = N # alias for N

    scores = W.dot(X.T).T
    #############################################################################
    #                              END OF YOUR CODE                             #
    #############################################################################
    
    # If the targets are not given then jump out, we're done
    if y is None:
      return scores

    # Compute the loss
    loss = None
    #############################################################################
    # TODO: Finish the forward pass, and compute the loss. This should include  #
    # both the data loss and L2 regularization for W. Store the result  	      # 
    # in the variable loss, which should be a scalar. Use the Softmax           #
    # classifier loss. So that your results match ours, multiply the            #
    # regularization loss by 0.5                                                #
    #############################################################################
    
    scores_total = np.sum(np.exp(scores.T), axis = 0)
    correct_class_scores = np.exp(scores[np.arange(number_of_training), y])
    loss = np.sum(np.log(correct_class_scores / scores_total) * (-1))
    loss /= number_of_training
    loss += 0.5 * reg * np.sum(W * W)

    
    #############################################################################
    #                              END OF YOUR CODE                             #
    #############################################################################

    # Backward pass: compute gradients
    grad = None
    #############################################################################
    # TODO: Compute the backward pass, computing the derivatives of the weights.#
    # Store the results in the `grad' variable, which should be a matrix that   #
    # has the same size as 'W' 							                                    #
    #############################################################################

    derivatives = (np.exp(scores.T) / scores_total).T # if j != y[i]
    derivatives[np.arange(number_of_training), y] = (np.exp(scores[np.arange(number_of_training), y]).T / scores_total).T - 1 # if j == y[i]
    grad = derivatives.T.dot(X)
    
    grad /= number_of_training
    grad += reg * W
    #############################################################################
    #                              END OF YOUR CODE                             #
    #############################################################################

    return loss, grad   
    

  def train(self, X, y, X_val, y_val,
            learning_rate=1e-3, learning_rate_decay=0.95,
            reg=1e-5, delta=1, num_iters=100,
            batch_size=200, loss='hinge', verbose=False):
    """
    Train this neural network using stochastic gradient descent.

    Inputs:
    - X: A numpy array of shape (N, D) giving training data.
    - y: A numpy array f shape (N,) giving training labels; y[i] = c means that
      X[i] has label c, where 0 <= c < C.
    - X_val: A numpy array of shape (N_val, D) giving validation data.
    - y_val: A numpy array of shape (N_val,) giving validation labels.
    - learning_rate: Scalar giving learning rate for optimization.
    - learning_rate_decay: Scalar giving factor used to decay the learning rate
      after each epoch.
    - reg: Scalar giving regularization strength.
    - num_iters: Number of steps to take when optimizing.
    - batch_size: Number of training examples to use per step.
    - loss: hinge or xentropy
    - verbose: boolean; if true print progress during optimization.
    """
    num_train = X.shape[0]
    iterations_per_epoch = max(num_train / batch_size, 1)

    # Use SGD to optimize the parameters in self.model
    loss_history = []
    train_acc_history = []
    val_acc_history = []


    merged = np.concatenate((X, y), axis=1)
    for it in xrange(num_iters):
      X_batch = None
      y_batch = None

      #########################################################################
      # TODO: Create a random minibatch of training data and labels, storing  #
      # them in X_batch and y_batch respectively.                             #
      #########################################################################
      """
      if idx_batch >= iterations_per_epoch:
        idx_batch = 0
      if (idx_batch + 1) * batch_size > num_train:
        replacement_factor = (idx_batch + 1) * batch_size - num_train
        batch_arr = np.arange(replacement_factor, num_train)
        idx_batch = 0
      else:
        batch_arr = np.arange(idx_batch * batch_size, (idx_batch + 1)*batch_size)
        idx_batch += 1

      X_batch = X[batch_arr]
      y_batch = y[batch_arr]
      """
      merged_batch = np.random.choice(merged, batch_size, replace=False)
      X_batch = merged_batch[:, 0]
      y_batch = merged_batch[:, 1]
      
      

      #########################################################################
      #                             END OF YOUR CODE                          #
      #########################################################################

      # Compute loss and gradients using the current minibatch
      if loss == 'hinge':
      	loss_val, grad = self.vectorized_hinge_loss(X_batch, y=y_batch, reg=reg, delta=delta)
      elif loss == 'xentropy':
      	loss_val, grad = self.vectorized_xentropy_loss(X_batch, y=y_batch, reg=reg)
      else:
      	print "Unknown loss function ('%s')" % (loss)
      
      loss_history.append(loss_val)

      #########################################################################
      # TODO: Use the gradients in the grads dictionary to update the         #
      # parameters of the network (stored in the dictionary self.params)      #
      # using stochastic gradient descent. You'll need to use the gradients   #
      # stored in the grad variable defined above.                            #
      #########################################################################
      self.params['W'] -= learning_rate * grad

      #########################################################################
      #                             END OF YOUR CODE                          #
      #########################################################################

      if verbose and it % 100 == 0:
        print 'iteration %d / %d: loss %f' % (it, num_iters, loss_val)

      # Every epoch, check train and val accuracy and decay learning rate.
      if it % iterations_per_epoch == 0:
        # Check accuracy
        train_acc = (self.predict(X_batch, loss) == y_batch).mean()
        val_acc = (self.predict(X_val, loss) == y_val).mean()
        train_acc_history.append(train_acc)
        val_acc_history.append(val_acc)

        # Decay learning rate
        learning_rate *= learning_rate_decay

    return {
      'loss_history': loss_history,
      'train_acc_history': train_acc_history,
      'val_acc_history': val_acc_history,
    }

  def predict(self, X, loss='hinge'):
    """
    Use the trained weights of the model to predict labels for
    data points. For each data point we predict scores for each of the C
    classes, and assign each data point to the class with the highest score.

    Inputs:
    - X: A numpy array of shape (N, D) giving N D-dimensional data points to
      classify.

    Returns:
    - y_pred: A numpy array of shape (N,) giving predicted labels for each of
      the elements of X. For all i, y_pred[i] = c means that X[i] is predicted
      to have class c, where 0 <= c < C.
    """
    y_pred = None

    ###########################################################################
    # TODO: Implement this function; it should be VERY simple!                #
    # Hint: Look up numpy's argmax function				                            #
    ###########################################################################
    W = self.params['W']
    scores = W.dot(X.T)
    y_pred = np.argmax(scores, axis=0)
    ###########################################################################
    #                              END OF YOUR CODE                           #
    ###########################################################################

    return y_pred


