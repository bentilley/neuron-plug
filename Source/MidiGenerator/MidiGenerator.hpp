/*
 * MidiGenerator.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Utils/PluginLogger.hpp"
#include "BeatClock/BeatClock.hpp"
#include "MidiProcessor/MidiProcessor.hpp"
#include "WellNeurons/Brain.hpp"
#include <memory>

class MidiGenerator {
public:
  MidiGenerator(int num_neurons);
  ~MidiGenerator();

  // Getters & Setters - mostly called on the GUI thread
  void toggleOnOff();
  bool get_is_on();
  void toggleReceivesMidi();
  bool get_receives_midi();

  int get_subdivision();
  void set_subdivision(int s);

  float get_volume();
  void set_volume(float v);
  int get_volume_clip_min();
  int get_volume_clip_max();
  void set_volume_clip(int min, int max);

  int get_neuron_midi_note(int neuron_idx);
  void set_neuron_midi_note(int neuron_idx, int new_note_number);
  int get_neuron_input_weight(int neuron_idx);
  void set_neuron_input_weight(int neuron_idx, int new_input_weight);
  int get_neuron_threshold(int neuron_idx);
  void set_neuron_threshold(int neuron_idx, int new_threshold);
  int get_neuron_connection_weight(int from, int to);
  void set_neuron_connection_weight(int from, int to,
                                    int new_connection_weight);

  // Neuron Model Methods
  int num_neurons();
  void add_neuron();
  void remove_neuron();
  void remove_neuron_at(int index);

  // Audio Thread
  void generate_next_midi_buffer(MidiBuffer &b,
                                 const AudioPlayHead::CurrentPositionInfo &pos,
                                 double sample_rate, int num_samples);

private:
  bool is_on, receives_midi;

  Brain brain;
  MidiProcessor midiProcessor;
  BeatClock beatClock;
};
