/*
 * Network.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Network.hpp"
#include <algorithm>
#include <iostream>

Network::Network(int starting_num_neurons)
{
  for (int i{0}; i < starting_num_neurons; ++i) {
    addNeuron();
  }
};
Network::~Network(){};

/*********************
 * Getters & Setters *
 *********************/

std::vector<int> Network::getOutput()
{
  std::vector<int> outputs(numNeurons(), 0);
  transform(neurons.begin(), neurons.end(), outputs.begin(), [](Neuron n) {
    return n.get_output();
  });
  return outputs;
};

std::vector<Neuron> Network::getNeurons() { return neurons; };

std::vector<std::vector<int>> Network::getConnectionWeights() { return connectionWeights; };

std::vector<int> Network::getInputWeights() { return inputWeights; }

void Network::setInputWeights(std::vector<int> new_weights)
{
  if (inputWeights.size() != new_weights.size()) {
    throw std::invalid_argument("input weights incorect shape");
  }
  inputWeights = new_weights;
}

int Network::getInputWeightForNeuron(int neuron_num) { return inputWeights.at(neuron_num); }

void Network::setInputWeightForNeuron(int neuron_num, int new_weight)
{
  inputWeights.at(neuron_num) = new_weight;
};

int Network::getConnectionWeightForNeurons(int from, int to)
{
  return connectionWeights.at(from).at(to);
}

void Network::setConnectionWeights(std::vector<std::vector<int>> new_weights)
{
  if (
    connectionWeights.size() != new_weights.size() ||
    connectionWeights.at(0).size() != new_weights.at(0).size()) {
    throw std::invalid_argument("connection weights incorect shape");
  }
  connectionWeights = new_weights;
}

void Network::setConnectionWeightForNeurons(int from, int to, int new_weight)
{
  connectionWeights.at(from).at(to) = new_weight;
};

int Network::getThresholdForNeuron(int neuron_num)
{
  return neurons.at(neuron_num).get_threshold();
}

void Network::setThresholdForNeuron(int neuron_num, int new_threshold)
{
  neurons.at(neuron_num).set_threshold(new_threshold);
};

/***********
 * Methods *
 ***********/

void Network::setNeuronMidiNotes(std::vector<int> notes)
{
  assert(notes.size() == midiNotes.size());
  midiNotes = notes;
}

int Network::getNeuronMidiNote(int neuron_idx) { return midiNotes.at(neuron_idx); }

void Network::setNeuronMidiNote(int neuron_idx, int new_note_number)
{
  midiNotes.at(neuron_idx) = new_note_number;
}

int Network::numNeurons() { return static_cast<int>(neurons.size()); };

void Network::addNeuron()
{
  neurons.push_back(Neuron());
  for (int i = 0; i < static_cast<int>(connectionWeights.size()); ++i) {
    connectionWeights.at(i).push_back(0);
  }
  connectionWeights.push_back(std::vector<int>(numNeurons(), 0));
  inputWeights.push_back(0);
  midiNotes.push_back(1);
};

void Network::removeNeuron()
{
  neurons.pop_back();
  inputWeights.pop_back();
  connectionWeights.pop_back();
  for (int i = 0; i < static_cast<int>(connectionWeights.size()); ++i) {
    connectionWeights.at(i).pop_back();
  }
  midiNotes.pop_back();
};

void Network::removeNeuronAt(int neuron_index)
{
  neurons.erase(neurons.begin() + neuron_index);
  inputWeights.erase(inputWeights.begin() + neuron_index);
  connectionWeights.erase(connectionWeights.begin() + neuron_index);
  for (int i = 0; i < connectionWeights.size(); ++i) {
    std::vector<int>* n = &connectionWeights.at(i);
    n->erase(n->begin() + neuron_index);
  }
  midiNotes.erase(midiNotes.begin() + neuron_index);
};

std::vector<int> Network::getWeightedInput(std::vector<int> input)
{
  assert(input.size() == numNeurons());
  std::vector<int> weighted_input(numNeurons(), 0);
  for (int i = 0; i < input.size(); ++i) {
    weighted_input.at(i) = input.at(i) * inputWeights.at(i);
  }
  return weighted_input;
};

std::vector<int> Network::getConnectionEnergy(std::vector<int> output)
{
  assert(output.size() == numNeurons());
  std::vector<int> connection_energy(numNeurons(), 0);
  for (int i = 0; i < numNeurons(); ++i) {
    int total_connection_energy = 0;
    for (int j = 0; j < numNeurons(); ++j) {
      total_connection_energy += connectionWeights.at(j).at(i) * output.at(j);
    }
    connection_energy.at(i) = total_connection_energy;
  }
  return connection_energy;
};

std::vector<int> Network::compressInput(ModelVector input)
{
  std::vector<int> compressed(numNeurons(), 0);
  for (int i = 0; i < numNeurons(); ++i) {
    compressed.at(i) = input.data.at(midiNotes.at(i));
  }
  return compressed;
};

std::vector<int> Network::getNeuronInput(std::vector<int> input, std::vector<int> prev_output)
{
  std::vector<int> neuron_input(numNeurons(), 0);
  std::vector<int> weighted_input = getWeightedInput(input);
  std::vector<int> connection_energy = getConnectionEnergy(prev_output);
  for (int i = 0; i < neurons.size(); ++i) {
    int next_input = weighted_input.at(i) + connection_energy.at(i);
    neuron_input.at(i) = next_input;
  }
  return neuron_input;
}

void Network::neuronsUpdateState(std::vector<int> input)
{
  assert(input.size() == numNeurons());
  for (int i = 0; i < neurons.size(); ++i) {
    neurons.at(i).update_state(input.at(i));
  }
};

ModelVector Network::expandOutput(ModelVector& origInput, std::vector<int> output)
{
  assert(output.size() == numNeurons());
  ModelVector expanded(0, origInput.sampleNumber, origInput.inputType);
  for (int i = 0; i < output.size(); ++i) {
    expanded.data.at(midiNotes.at(i)) = output.at(i);
  }
  return expanded;
};

ModelVector Network::processNext(ModelVector input)
{
  auto compressed = compressInput(input);
  auto neuron_inputs = getNeuronInput(compressed, getOutput());
  neuronsUpdateState(neuron_inputs);
  return expandOutput(input, getOutput());
};
