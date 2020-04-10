/*
 * MidiProcessor.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiProcessor.hpp"
#include "assert.h"

MidiProcessor::MidiProcessor() { midi_map = std::vector<int>{60, 64, 67}; };
MidiProcessor::~MidiProcessor(){};

std::vector<int> MidiProcessor::get_midi_map() { return midi_map; }

void MidiProcessor::render_buffer(MidiBuffer &buffer,
                                  std::vector<int> new_output, int sample_num) {
  assert(midi_map.size() == new_output.size());

  for (int i = 0; i < new_output.size(); ++i) {
    if (new_output.at(i) == 1) {
      int midi_note_num = midi_map.at(i);
      MidiMessage m = MidiMessage::noteOn(1, midi_note_num, (uint8)100);
      buffer.addEvent(m, sample_num);
    }
  }
}
