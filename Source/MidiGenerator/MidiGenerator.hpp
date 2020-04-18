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

class MidiGenerator {
public:
  MidiGenerator();
  ~MidiGenerator();

  // Getters
  bool get_is_on();

  // Audio Thread
  void generate_next_midi_buffer(MidiBuffer &b,
                                 const AudioPlayHead::CurrentPositionInfo &pos,
                                 double sample_rate, int num_samples);

  // GUI Thread
  void toggleOnOff();

private:
  bool is_on;

  Brain brain;
  MidiProcessor midiProcessor;
  BeatClock beatClock;
};
