/*
 * MidiProcessor.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiProcessor.hpp"
#include "assert.h"
#include <cmath>

MidiProcessor::MidiProcessor(int num_notes)
    : max_brain_output{1}, global_volume{1.0}, volume_clip{1, 127} {
  midi_map = std::vector<int>(num_notes, 1);
};
MidiProcessor::~MidiProcessor(){};

// Getters & Setters

float MidiProcessor::get_global_volume() { return global_volume; };
void MidiProcessor::set_global_volume(float v) { global_volume = v; };
int MidiProcessor::get_volume_clip_min() { return volume_clip.first; };
int MidiProcessor::get_volume_clip_max() { return volume_clip.second; };
void MidiProcessor::set_volume_clip(int min, int max) {
  assert(min <= max);
  volume_clip = std::make_pair(min, max);
};
int MidiProcessor::get_volume_clip_range() {
  return volume_clip.second - volume_clip.first;
}

std::vector<int> MidiProcessor::get_midi_map() { return midi_map; }
int MidiProcessor::get_note_at(int neuron_idx) {
  return midi_map.at(neuron_idx);
}
void MidiProcessor::set_note_at(int neuron_idx, int new_note_number) {
  midi_map.at(neuron_idx) = new_note_number;
  PluginLogger::logger.log_vec("midi notes", midi_map);
}

// Methods

int MidiProcessor::clip_brain_output(int output) {
  return output <= max_brain_output ? output : max_brain_output;
}
float MidiProcessor::as_percent_of_max(int output) {
  return output / (float)max_brain_output;
}
int MidiProcessor::get_clipped_midi_volume(float vel) {
  if (vel < 0.001)
    return 0;
  return std::round(volume_clip.first + (get_volume_clip_range() * vel));
}

uint8 MidiProcessor::get_note_velocity(int brain_output_value) {
  float output = as_percent_of_max(clip_brain_output(brain_output_value));
  return std::round(get_clipped_midi_volume(output) * global_volume);
}

void MidiProcessor::add_midi_note() { midi_map.push_back(1); }
void MidiProcessor::add_midi_note(int note_num) {
  midi_map.push_back(note_num);
}
void MidiProcessor::remove_midi_note() { midi_map.pop_back(); }
void MidiProcessor::remove_midi_note_at(int index) {
  midi_map.erase(midi_map.begin() + index);
}

void MidiProcessor::render_buffer(MidiBuffer &buffer,
                                  std::vector<int> new_output, int sample_num) {
  assert(midi_map.size() == new_output.size());

  for (int i = 0; i < new_output.size(); ++i) {
    if (new_output.at(i) == 1) {
      uint8 vel = get_note_velocity(new_output.at(i));
      int midi_note_num = midi_map.at(i);
      MidiMessage m = MidiMessage::noteOn(1, midi_note_num, vel);
      buffer.addEvent(m, sample_num);
    }
  }
}
