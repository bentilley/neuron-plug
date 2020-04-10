/*
 * BeatClock.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "BeatClock.hpp"

BeatClock::BeatClock() {
  subdivision = 1;
  _is_configured = false;
};
BeatClock::~BeatClock(){};

/*
 * Getters
 */

int BeatClock::get_subdivision() { return subdivision; }
bool BeatClock::is_configured() { return _is_configured; }
float BeatClock::get_samples_per_subdivision() {
  return samples_per_subdivision;
}
float BeatClock::get_sample_num_remainder() { return sample_num_remainder; }

/*
 * Setters
 */

void BeatClock::set_subdivision(int new_subdiv) { subdivision = new_subdiv; }

/*
 * Public Methods
 */

void BeatClock::configure(double sample_rate, posinfo &pos) {
  float bpm = (float)pos.bpm;
  double current_sample_num = (double)pos.timeInSamples;
  samples_per_subdivision = get_samples_per_subdivision(bpm, sample_rate);
  sample_num_remainder =
      get_sample_num_remainder(samples_per_subdivision, current_sample_num);
  _is_configured = true;
}

bool BeatClock::should_play(int buffer_sample_num) {
  assert(_is_configured);
  float current_sample_remainder = sample_num_remainder + buffer_sample_num;
  if (fmod(current_sample_remainder, samples_per_subdivision) < 1) {
    return true;
  } else {
    return false;
  }
}

void BeatClock::reset() { _is_configured = false; }

/*
 * Private Methods
 */

float BeatClock::get_samples_per_subdivision(float bpm, double sample_rate) {
  return ((60.0 / bpm) / (float)subdivision) * sample_rate;
}

float BeatClock::get_sample_num_remainder(float samples_per_subdivision,
                                          double current_sample_num) {
  /* float bpm = (float)pos.bpm; */
  /* double current_sample_num = (double)pos.timeInSamples; */

  /* float samples_per_subdivision = get_samples_per_subdivision(bpm,
   * sample_rate); */
  return fmod(current_sample_num, samples_per_subdivision);
}
