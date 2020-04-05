/*
 * Brain.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Brain.hpp"
#include <iostream>

Brain::Brain() {
  neurons = std::vector<Neuron>{Neuron(), Neuron(), Neuron()};
  input_weights = std::vector<int>{1, 1, 1};
  connection_weights = std::vector<std::vector<int>>{std::vector<int>{0, 0, 0},
                                                     std::vector<int>{0, 0, 0},
                                                     std::vector<int>{0, 0, 0}};
};
Brain::~Brain(){};

/*
 * Getters
 */

std::vector<int> Brain::get_output() {
  std::vector<int> outputs(num_neurons(), 0);
  transform(neurons.begin(), neurons.end(), outputs.begin(),
            [](Neuron n) { return n.get_output(); });
  return outputs;
};

std::vector<Neuron> Brain::get_neurons() { return neurons; };

std::vector<std::vector<int>> Brain::get_connection_weights() {
  return connection_weights;
};

std::vector<int> Brain::get_input_weights() { return input_weights; }

/*
 * Setters
 */

void Brain::set_input_weights(std::vector<int> new_weights) {
  if (input_weights.size() != new_weights.size()) {
    throw std::invalid_argument("input weights incorect shape");
  }
  input_weights = new_weights;
}

void Brain::set_connection_weights(std::vector<std::vector<int>> new_weights) {
  if (connection_weights.size() != new_weights.size() ||
      connection_weights.at(0).size() != new_weights.at(0).size()) {
    throw std::invalid_argument("connection weights incorect shape");
  }
  connection_weights = new_weights;
}

void Brain::set_connection_weight_for_neurons(int from, int to,
                                              int new_weight) {
  int from_idx = from - 1, to_idx = to - 1;
  connection_weights.at(from_idx).at(to_idx) = new_weight;
};

void Brain::set_input_weight_for_neuron(int neuron_num, int new_weight) {
  int neuron_idx = neuron_num - 1;
  input_weights.at(neuron_idx) = new_weight;
};

void Brain::set_threshold_for_neuron(int neuron_num, int new_threshold) {
  int neuron_idx = neuron_num - 1;
  neurons.at(neuron_idx).set_threshold(new_threshold);
};

/*
 * Methods
 */

int Brain::num_neurons() { return static_cast<int>(neurons.size()); };

void Brain::add_neuron() {
  neurons.push_back(Neuron());
  for (int i = 0; i < connection_weights.size(); ++i) {
    connection_weights.at(i).push_back(0);
  }
  connection_weights.push_back(std::vector<int>(num_neurons(), 0));
  input_weights.push_back(0);
};

void Brain::remove_neuron() {
  neurons.pop_back();
  input_weights.pop_back();
  connection_weights.pop_back();
  for (int i = 0; i < connection_weights.size(); ++i) {
    connection_weights.at(i).pop_back();
  }
};

void Brain::remove_neuron(int neuron_number) {
  int neuron_index = neuron_number - 1;
  neurons.erase(neurons.begin() + neuron_index);
  input_weights.erase(input_weights.begin() + neuron_index);
  connection_weights.erase(connection_weights.begin() + neuron_index);
  for (int i = 0; i < connection_weights.size(); ++i) {
    std::vector<int> *n = &connection_weights.at(i);
    n->erase(n->begin() + neuron_index);
  }
};

std::vector<int> Brain::get_weighted_input(std::vector<int> input) {
  assert(input.size() == num_neurons());
  std::vector<int> weighted_input(num_neurons(), 0);
  for (int i = 0; i < input.size(); ++i) {
    weighted_input.at(i) = input.at(i) * input_weights.at(i);
  }
  return weighted_input;
};

std::vector<int> Brain::get_connection_energy(std::vector<int> output) {
  assert(output.size() == num_neurons());
  std::vector<int> connection_energy(num_neurons(), 0);
  for (int i = 0; i < num_neurons(); ++i) {
    int total_connection_energy = 0;
    for (int j = 0; j < num_neurons(); ++j) {
      total_connection_energy += connection_weights.at(j).at(i) * output.at(j);
    }
    connection_energy.at(i) = total_connection_energy;
  }
  return connection_energy;
};

void Brain::input_to_neurons(std::vector<int> input,
                             std::vector<int> prev_output) {
  std::vector<int> weighted_input = get_weighted_input(input);
  std::vector<int> connection_energy = get_connection_energy(prev_output);
  for (int i = 0; i < neurons.size(); ++i) {
    int next_input = weighted_input.at(i) + connection_energy.at(i);
    neurons.at(i).set_input(next_input);
  }
}

void Brain::neurons_update_state() {
  for_each(neurons.begin(), neurons.end(), [](Neuron &n) { n.update_state(); });
};

std::vector<int> Brain::process_next(std::vector<int> input) {
  input_to_neurons(input, get_output());
  neurons_update_state();
  return get_output();
};
