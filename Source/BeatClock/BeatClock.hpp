/*
 * BeatClock.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

typedef AudioPlayHead::CurrentPositionInfo posinfo;

class BeatClock {
public:
  BeatClock();
  ~BeatClock();

  int get_subdivision();
  bool is_configured();
  float get_samples_per_subdivision();
  float get_sample_num_remainder();

  void set_subdivision(int new_subdivision);

  void configure(double sample_rate, posinfo &pos);
  bool should_play(int buffer_sample_num);
  void reset();

private:
  int subdivision;
  bool _is_configured;
  float samples_per_subdivision;
  float sample_num_remainder;

  float get_samples_per_subdivision(float bpm, double sample_rate);
  float get_sample_num_remainder(float samples_per_subdivision,
                                 double current_sample_num);
};
