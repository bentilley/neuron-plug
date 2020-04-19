/*
 * MidiProcessor.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Utils/PluginLogger.hpp"
#include <utility>
#include <vector>

class MidiProcessor {
public:
  MidiProcessor();
  ~MidiProcessor();

  // Getters & Setters
  float get_global_volume();
  void set_global_volume(float v);
  int get_volume_clip_min();
  int get_volume_clip_max();
  void set_volume_clip(int min, int max);
  int get_volume_clip_range();

  std::vector<int> get_midi_map();
  int get_note_at(int neuron_idx);
  void set_note_at(int neuron_idx, int new_note_number);

  // Methods
  int clip_brain_output(int output);
  float as_percent_of_max(int output);
  int get_clipped_midi_volume(float vel);
  uint8 get_note_velocity(int brain_output_value);

  void render_buffer(MidiBuffer &buffer, std::vector<int> next_output,
                     int sample_num);

private:
  int max_brain_output;
  float global_volume;
  std::pair<int, int> volume_clip;
  std::vector<int> midi_map;
};
