from tflite_runtime.interpreter import Interpreter
import numpy as np
import cv2

from typing import List, NamedTuple

class VideoClassifierOptions(NamedTuple):
  """A config to initialize an video classifier."""

  max_results: int = 1
  """The maximum number of top-scored classification results to return."""

  num_threads: int = 4
  """The number of CPU threads to be used."""

  score_threshold: float = 0.0
  """The score threshold of classification results to return."""

class VideoClassifier(object):
  """A wrapper class for a TFLite video classification model."""

  _MODEL_INPUT_SIGNATURE_NAME = 'image'
  _MODEL_OUTPUT_SIGNATURE_NAME = 'logits'
  _MODEL_INPUT_MEAN = 0
  _MODEL_INPUT_STD = 255

  def __init__(
      self,
      model_path: str,
      label_file: str,
      options: VideoClassifierOptions = VideoClassifierOptions()
  ) -> None:
    """Initialize a video classification model.

    Args:
        model_path: Path of the TFLite video classification model.
        label_file: Path of the video classification label list.
        options: The config to initialize an video classifier. (Optional)

    Raises:
        ValueError: If the TFLite model is invalid.
    """

    interpreter = Interpreter(
        model_path=model_path, num_threads=options.num_threads)
    signature = interpreter.get_signature_runner()

    # Load the label list.
    with open(label_file, 'r') as f:
      lines = f.readlines()
      label_list = [line.replace('\n', '') for line in lines]
      self._label_list = label_list
    
    # Remove the batch dimension to get the real input shape.
    input_shape = signature.get_input_details()[
        self._MODEL_INPUT_SIGNATURE_NAME]['shape']
    input_shape = np.delete(input_shape, np.where(input_shape == 1))
    self._input_height = input_shape[0]
    self._input_width = input_shape[1]

    # Store the signature runner and model options for later use.
    self._signature = signature
    self._options = options

    # Set the initial state for the model.
    self._internal_states = {}
    self.clear()

  def clear(self):
    """Clear the internal state of the model to start classifying a new scene."""
    # Create the initial (zero) states
    init_states = {
        name: np.zeros(signature['shape'], dtype=signature['dtype'])
        for name, signature in self._signature.get_input_details().items()
    }

    # Remove the holder for the input image as it'll be fed by the caller.
    init_states.pop(self._MODEL_INPUT_SIGNATURE_NAME)

    # Store the model's internal state.
    self._internal_states = init_states

  def _preprocess(self, image: np.ndarray) -> np.ndarray:
    """Preprocess the image as required by the TFLite model."""
    input_tensor = cv2.resize(image, (self._input_width, self._input_height))
    input_tensor = input_tensor[np.newaxis, np.newaxis]
    input_tensor = np.float32(input_tensor -
                              self._MODEL_INPUT_MEAN) / self._MODEL_INPUT_STD

    return input_tensor

  def classify(self, frame: np.ndarray):
    """Classify an input frame.

    Frames from the target video should be fed to the model in sequence.

    Args:
        frame: A [height, width, 3] RGB image representing a frame in a video.

    Returns:
        A list of prediction result. Sorted by probability descending.
    """
    # Preprocess the input frame.
    frame = self._preprocess(frame)
    # Feed the input frame and the model internal states to the TFLite model.
    outputs = self._signature(**self._internal_states, image=frame)

    # Take the model output and store the internal states for subsequence
    # frames.
    logits = outputs.pop(self._MODEL_OUTPUT_SIGNATURE_NAME)
    self._internal_states = outputs

    return self._postprocess(logits)

  def _postprocess(self, logits: np.ndarray):
    """Post-process the logits into a list of Category objects.

    Args:
        logits: Raw logits output of the TFLite model.

    Returns:
        A list of classification results.
    """
    # Convert from logits to probabilities using softmax function.
    exp_logits = np.exp(np.squeeze(logits, axis=0))
    probabilities = exp_logits / np.sum(exp_logits)
    
    # Sort the labels so that the more likely categories come first.
    prob_descending = sorted(
        range(len(probabilities)), key=lambda k: probabilities[k], reverse=True)
    labels_scores = [(self._label_list[idx], probabilities[idx]) for idx in prob_descending]

    # Only return maximum of max_results categories
    return labels_scores[0][0]
