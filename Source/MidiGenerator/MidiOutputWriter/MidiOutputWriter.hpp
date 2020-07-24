/*
 * MIDIOutputWriter.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Brain/io.hpp"
#include <vector>

class MidiOutputWriter {
public:
  MidiOutputWriter() {}

  MidiBuffer &writeMidiOutput(std::vector<ModelVector> &output,
                              MidiBuffer &buffer);

private:
  std::array<uint_least8_t, NUM_MIDI_NOTES> playingMidiNotes;
};

/** A function to reduce the ModelVector sample numbers modulo the buffer size.
 *
 * The ModelVector's hold an absolute sample number. When the vectors are
 * written to the MidiBuffer, the sample numbers used need to be relative to the
 * buffer, not the start of the time line. This is a convenience function to
 * reduce the sample numbers modulo the buffer size, before passing the result
 * to the MidiOutputWriter.
 *
 * @param output The output vectors of the model.
 * @param bufferSize The number of samples in the current buffer.
 */
std::vector<ModelVector> &
sampleNumModuloBufferSize(std::vector<ModelVector> &output, int bufferSize);
