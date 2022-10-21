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

float Neuron::getState() { return state; };
float Neuron::getThreshold() { return threshold; };

float Neuron::getOutput()
{
  if (state - threshold > 0) {
    return 1;
  } else {
    return 0;
  }
};

void Neuron::setThreshold(float new_threshold) { threshold = new_threshold; };

void Neuron::updateState(float input) { state = state + input; };
