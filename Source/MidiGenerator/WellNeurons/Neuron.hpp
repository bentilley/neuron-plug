/*
 * Neuron.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <vector>

/** A simple neuron function that can be combined to form a more complex
 * network.
 *
 * A neuron has state, takes input, updates its state, and produces output.
 */
class Neuron
{
public:
  //============================================================================
  /** Default constructor */
  Neuron();
  /** Default destructor */
  ~Neuron();

  //============================================================================
  /** Get the neuron output, based on the state and the threshold. */
  int get_output();
  /** Get the neuron state. */
  int get_state();
  /** Get the neuron threshold. */
  int get_threshold();

  /** Set the neuron threshold.
   *
   * @param new_threshold The new threshold of the neuron.
   */
  void set_threshold(int new_threshold);

  /** Calculate the new state of the neuron.
   *
   * This is based on the current state of the neuron and the input.
   *
   * @param input The current input to the neuron.
   */
  void update_state(int input);

private:
  //============================================================================
  /** The current state of the neuron. */
  int state{0};
  /** The firing threshold of the neuron.
   *
   * In order for the neuron to produce output, the value of the state must be
   * greater than the threshold.
   */
  int threshold{0};
};
