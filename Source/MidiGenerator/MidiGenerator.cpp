/*
 * MidiGenerator.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiGenerator.hpp"

MidiGenerator::MidiGenerator() : is_on{false}, receives_midi{false} {
  brain.set_connection_weights(std::vector<std::vector<int>>{
      std::vector<int>{-5, 2, 1}, std::vector<int>{1, -6, 2},
      std::vector<int>{2, 1, -7}});
}
MidiGenerator::~MidiGenerator() {}

/*
 * Getters & Setters
 */

void MidiGenerator::toggleOnOff() { is_on = !is_on; };
bool MidiGenerator::get_is_on() { return is_on; };
void MidiGenerator::toggleReceivesMidi() { receives_midi = receives_midi; };
bool MidiGenerator::get_receives_midi() { return receives_midi; };

int MidiGenerator::get_subdivision() { return beatClock.get_subdivision(); }
void MidiGenerator::set_subdivision(int s) { beatClock.set_subdivision(s); }

float MidiGenerator::get_volume() { return midiProcessor.get_global_volume(); };
void MidiGenerator::set_volume(float v) { midiProcessor.set_global_volume(v); };
int MidiGenerator::get_volume_clip_min() {
  return midiProcessor.get_volume_clip_min();
};
int MidiGenerator::get_volume_clip_max() {
  return midiProcessor.get_volume_clip_max();
};
void MidiGenerator::set_volume_clip(int min, int max) {
  midiProcessor.set_volume_clip(min, max);
};

/*
 * Audio Thread
 */

void MidiGenerator::generate_next_midi_buffer(
    MidiBuffer &midiBuffer, const AudioPlayHead::CurrentPositionInfo &pos,
    double sample_rate, int num_samples) {

  beatClock.configure(sample_rate, pos);

  for (int time = 0; time < num_samples; ++time) {
    if (beatClock.should_play(time)) {
      brain.process_next(std::vector<int>{1, 1, 1});
      std::vector<int> output = brain.get_output();
      PluginLogger::logger.log_vec("model output", output);

      midiProcessor.render_buffer(midiBuffer, output, time);
    }
  }

  beatClock.reset();
};
