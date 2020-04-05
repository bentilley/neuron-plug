/*
 * MidiProcessor.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiProcessor.hpp"

MidiProcessor::MidiProcessor(){};
MidiProcessor::~MidiProcessor(){};

void MidiProcessor::render_buffer(MidiBuffer &buffer,
                                  std::vector<int> new_output) {
  printf("render_buffer\n");
}

bool MidiProcessor::test() { return true; }

bool MidiProcessor::testFail() { return false; }
