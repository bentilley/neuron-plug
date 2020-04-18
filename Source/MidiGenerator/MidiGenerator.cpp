/*
 * MidiGenerator.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiGenerator.hpp"

MidiGenerator::MidiGenerator() : is_on{false} {
  brain.set_connection_weights(std::vector<std::vector<int>>{
      std::vector<int>{-5, 2, 1}, std::vector<int>{1, -6, 2},
      std::vector<int>{2, 1, -7}});
}
MidiGenerator::~MidiGenerator() {}

/*
 * Getters
 */

bool MidiGenerator::get_is_on() { return is_on; };

/*
 * Audio Thread
 */

void MidiGenerator::generate_next_midi_buffer(
    MidiBuffer &midiBuffer, AudioPlayHead::CurrentPositionInfo &pos,
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

/*
 * GUI Thread
 */

void MidiGenerator::toggleOnOff() { is_on = !is_on; };
