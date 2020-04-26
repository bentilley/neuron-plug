/*
 * MidiGenerator.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiGenerator.hpp"

MidiGenerator::MidiGenerator() : is_on{false}, receives_midi{false} {}
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

// MIDI Notes
int MidiGenerator::get_neuron_midi_note(int neuron_idx) {
  return midiProcessor.get_note_at(neuron_idx);
}
void MidiGenerator::set_neuron_midi_note(int neuron_idx, int new_note_number) {
  midiProcessor.set_note_at(neuron_idx, new_note_number);
}

// Input Weight
int MidiGenerator::get_neuron_input_weight(int neuron_idx) {
  return brain.get_input_weight_for_neuron(neuron_idx);
}
void MidiGenerator::set_neuron_input_weight(int neuron_idx,
                                            int new_input_weight) {
  brain.set_input_weight_for_neuron(neuron_idx, new_input_weight);
}

// Threshold
int MidiGenerator::get_neuron_threshold(int neuron_idx) {
  return brain.get_threshold_for_neuron(neuron_idx);
}
void MidiGenerator::set_neuron_threshold(int neuron_idx, int new_threshold) {
  brain.set_threshold_for_neuron(neuron_idx, new_threshold);
}

// Connection Weights
int MidiGenerator::get_neuron_connection_weight(int from, int to) {
  return brain.get_connection_weight_for_neurons(from, to);
}
void MidiGenerator::set_neuron_connection_weight(int from, int to,
                                                 int new_connection_weight) {
  brain.set_connection_weight_for_neurons(from, to, new_connection_weight);
}

/*
 * Neuron Model Methods
 */

int MidiGenerator::num_neurons() { return brain.num_neurons(); };

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
