/*
 * MidiOutputWriter.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiOutputWriter.hpp"
#include <iostream>

MidiBuffer &MidiOutputWriter::writeMidiOutput(std::vector<ModelVector> &output,
                                              MidiBuffer &buffer) {
  return buffer;
}

std::vector<ModelVector> &
sampleNumModuloBufferSize(std::vector<ModelVector> &output, int bufferSize) {
  for (auto &modelVector : output) {
    modelVector.sampleNumber = modelVector.sampleNumber % bufferSize;
  }
  return output;
}
