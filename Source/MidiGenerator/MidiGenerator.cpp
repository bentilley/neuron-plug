/*
 * MidiGenerator.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiGenerator.hpp"

MidiGenerator::MidiGenerator(int num_neurons)
    : is_on{false}, receives_midi{false}, network(num_neurons),
      midiProcessor(num_neurons) {}
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
  PluginLogger::logger.log_vec("input weights", network.get_input_weights());
  return network.get_input_weight_for_neuron(neuron_idx);
}
void MidiGenerator::set_neuron_input_weight(int neuron_idx,
                                            int new_input_weight) {
  network.set_input_weight_for_neuron(neuron_idx, new_input_weight);
  PluginLogger::logger.log_vec("input weights", network.get_input_weights());
}

// Threshold
int MidiGenerator::get_neuron_threshold(int neuron_idx) {
  return network.get_threshold_for_neuron(neuron_idx);
}
void MidiGenerator::set_neuron_threshold(int neuron_idx, int new_threshold) {
  network.set_threshold_for_neuron(neuron_idx, new_threshold);
  std::vector<int> thresholds(num_neurons(), 0);
  std::vector<Neuron> neurons = network.get_neurons();
  std::transform(neurons.begin(), neurons.end(), thresholds.begin(),
                 [](Neuron n) { return n.get_threshold(); });
  PluginLogger::logger.log_vec("thresholds", thresholds);
}

// Connection Weights
int MidiGenerator::get_neuron_connection_weight(int from, int to) {
  return network.get_connection_weight_for_neurons(from, to);
}
void MidiGenerator::set_neuron_connection_weight(int from, int to,
                                                 int new_connection_weight) {
  network.set_connection_weight_for_neurons(from, to, new_connection_weight);
  PluginLogger::logger.log_vec("Connection weights from " + String(from),
                               network.get_connection_weights().at(from));
}

/*
 * Neuron Model Methods
 */

int MidiGenerator::num_neurons() { return network.num_neurons(); };

void MidiGenerator::add_neuron() {
  network.add_neuron();
  midiProcessor.add_midi_note(1);
}
void MidiGenerator::remove_neuron() {
  network.remove_neuron();
  midiProcessor.remove_midi_note();
}
void MidiGenerator::remove_neuron_at(int index) {
  network.remove_neuron_at(index);
  midiProcessor.remove_midi_note_at(index);
}

/*
 * Audio Thread
 */

void MidiGenerator::generate_next_midi_buffer(
    MidiBuffer &midiBuffer, const AudioPlayHead::CurrentPositionInfo &pos,
    double sample_rate, int num_samples) {

  beatClock.configure(sample_rate, pos);

  for (int time = 0; time < num_samples; ++time) {
    if (beatClock.should_play(time)) {
      network.process_next(std::vector<int>(num_neurons(), 1));
      std::vector<int> output = network.get_output();
      // TODO remove model output logging
      PluginLogger::logger.log_vec("model output", output);
      // TODO get neuron output for James to analyse

      midiProcessor.render_buffer(midiBuffer, output, time);
    }
  }

  beatClock.reset();
};
