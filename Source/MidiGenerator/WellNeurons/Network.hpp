/*
 * Network.hpp
 * Copyright (C) 2020 MrSquee <MrSquee@Mr-Squees-MBP>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../Brain/io.hpp"
#include "Neuron.hpp"
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

/** Takes a ModelVector as input and computes the next ModelVector to output.
 *
 * This is the brain behind the plugin. We create a vector that represents what
 * has just happened and feed it to this network. The network is then
 * responsible for outputting a vector that will be used to generate the MIDI
 * output of the plugin.
 */
class Network
{
public:
  //============================================================================
  /** Constructor to set number of initial neurons */
  Network(int starting_num_neurons);
  //** Destructor */
  ~Network();

  //============================================================================
  /** Get a vector of the outputs of the neurons. */
  std::vector<int> getOutput();
  /** Get the Network Neurons. */
  std::vector<Neuron> getNeurons();

  /** Get the values used to weight the input vector. */
  std::vector<int> getInputWeights();
  /** Set the values used to weight the input vector.
   *
   * @param newWeights The new weights to set.
   */
  void setInputWeights(std::vector<int> newWeights);

  /** Get the values used to weight the connections between neurons. */
  std::vector<std::vector<int>> getConnectionWeights();
  /** Set the values used to weight the connections between neurons.
   *
   * @param newWeights The new weights to set.
   */
  void setConnectionWeights(std::vector<std::vector<int>> newWeights);

  /** Get the value used to weight the input for a specific neuron.
   *
   * @param neuronNum The index of the neuron to get the input weight for.
   */
  int getInputWeightForNeuron(int neuronNum);
  /** Set the value used to weight the input for a specific neuron.
   *
   * @param neuronNum The index of the neuron to set the input weight for.
   * @param newWeight The new value to set the weight to.
   */
  void setInputWeightForNeuron(int neuronNum, int newWeight);

  /** Get the value of the connection weight between two neurons.
   *
   * @param from The index of the from neuron.
   * @param to The index of the to neuron.
   */
  int getConnectionWeightForNeurons(int from, int to);
  /** Set the valu of the connection weight between two neurons.
   *
   * @param from The index of the from neuron.
   * @param to The index of the to neuron.
   * @param newWeight The new value to set the weight to.
   */
  void setConnectionWeightForNeurons(int from, int to, int newWeight);

  /** Get the threshold value for a specific neuron.
   *
   * @param neuronNum The index of the neuron to get the threshold for.
   */
  int getThresholdForNeuron(int neuronNum);
  /** Set the threshold value for a specific neuron.
   *
   * @param neuronNum The index of the neuron to set the threshold for.
   * @param newThreshold The new value to set the threshold to.
   */
  void setThresholdForNeuron(int neuronNum, int newThreshold);

  /** Add a new neuron to the network with default parameter values. */
  void addNeuron();
  /** Remove the last added neuron from the network. */
  void removeNeuron();
  /** Remove a specific neuron from the network.
   *
   * @param neuronIndex The index of the neuron to remove.
   */
  void removeNeuronAt(int neuronIndex);

  /** Set the MIDI notes for all neurons at once.
   *
   * @param notes The new notes to set.
   */
  void setNeuronMidiNotes(std::vector<int> notes);
  /** Get the MIDI note for a specific neuron.
   *
   * @param neuronIdx The index of the Neuron to get the MIDI note for.
   */
  int getNeuronMidiNote(int neuronIdx);
  /** Set the MIDI note for a specific neuron.
   *
   * @param neuronIdx The index of the Neuron to get the MIDI note for.
   * @param newNoteNumber The number of the new MIDI node.
   */
  void setNeuronMidiNote(int neuronIdx, int newNoteNumber);

  /** Get the current number of neurons in the network. */
  int numNeurons();
  /** Apply the input weights to the network input.
   *
   * @param input The input vector to weight.
   */
  std::vector<int> getWeightedInput(std::vector<int> input);
  /** Get the energy passed between neurons.
   *
   * @param output The output vector to distribute between neurons.
   */
  std::vector<int> getConnectionEnergy(std::vector<int> output);
  /** Process a ModelVector input and return a ModelVector output.
   *
   * This is the main processing method for the network. It takes the input and
   * calculates the next output based on the state of the neurons.
   *
   * @param input The input to the network.
   */
  ModelVector processNext(ModelVector input);

private:
  //============================================================================
  /** The neurons of the network. */
  std::vector<Neuron> neurons;
  /** The values used to weight the input to the network. */
  std::vector<int> inputWeights;
  /** The values used to determine the connections between neurons in the network. */
  std::vector<std::vector<int>> connectionWeights;
  /** The MIDI notes that correspond to each neuron. */
  std::vector<int> midiNotes;

  /** Take the model input and convert it to a vector of the appropriate size
   * for the network.
   *
   * @param input The input vector to the network.
   */
  std::vector<int> compressInput(ModelVector input);

  /** Calculate the new input to the neurons.
   *
   * @param input The input to the network.
   * @param prevOutput The output of the network at time `t-1`.
   */
  std::vector<int> getNeuronInput(std::vector<int> input, std::vector<int> prevOutput);

  /** Each neuron calculates its new state based on its current state and
   * current input.
   */
  void neuronsUpdateState(std::vector<int> input);

  /** Take the network output and convert it back to the size of ModelVector.
   *
   * @param output The output vector of the network.
   */
  ModelVector expandOutput(ModelVector& origInput, std::vector<int> output);
};
