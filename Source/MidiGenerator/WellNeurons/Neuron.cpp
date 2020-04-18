/*
 * Neuron.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Neuron.hpp"
#include <cstdio>

Neuron::Neuron(){};
Neuron::~Neuron(){};

int Neuron::get_input() { return input; };
int Neuron::get_state() { return state; };
int Neuron::get_threshold() { return threshold; };

int Neuron::get_output() {
  if (state - threshold > 0) {
    return 1;
  } else {
    return 0;
  }
};

void Neuron::set_input(int new_input) { input = new_input; };
void Neuron::set_threshold(int new_threshold) { threshold = new_threshold; };

void Neuron::update_state() {
  state = state + input;
  input = 0;
};
