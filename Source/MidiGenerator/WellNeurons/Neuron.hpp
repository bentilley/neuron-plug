/*
 * Neuron.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <vector>

class Neuron {
public:
  Neuron();
  ~Neuron();

  int get_input();
  int get_output();
  int get_state();
  int get_threshold();

  void set_input(int new_input);
  void set_threshold(int new_threshold);

  void update_state();

private:
  int input{0};
  int state{0};
  int threshold{0};
};
