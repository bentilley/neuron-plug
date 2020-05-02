/*
 * Brain.hpp
 * Copyright (C) 2020 MrSquee <MrSquee@Mr-Squees-MBP>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "Neuron.hpp"
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

class Brain {
public:
  Brain(int starting_num_neurons);
  ~Brain();

  std::vector<int> get_output();
  std::vector<Neuron> get_neurons();
  std::vector<int> get_input_weights();
  std::vector<std::vector<int>> get_connection_weights();

  void set_input_weights(std::vector<int> new_weights);
  void set_connection_weights(std::vector<std::vector<int>> new_weights);

  int get_input_weight_for_neuron(int neuron_num);
  int get_connection_weight_for_neurons(int from, int to);
  int get_threshold_for_neuron(int neuron_num);
  void set_input_weight_for_neuron(int neuron_num, int new_weight);
  void set_connection_weight_for_neurons(int from, int to, int new_weight);
  void set_threshold_for_neuron(int neuron_num, int new_threshold);

  void add_neuron();
  void remove_neuron(); // removes last added neuron
  void remove_neuron_at(int neuron_index);

  int num_neurons();
  std::vector<int> get_weighted_input(std::vector<int> input);
  std::vector<int> get_connection_energy(std::vector<int> output);
  void input_to_neurons(std::vector<int> input, std::vector<int> prev_output);
  void neurons_update_state();
  std::vector<int> process_next(std::vector<int> input);

private:
  std::vector<Neuron> neurons;
  std::vector<int> input_weights;
  std::vector<std::vector<int>> connection_weights;
};
