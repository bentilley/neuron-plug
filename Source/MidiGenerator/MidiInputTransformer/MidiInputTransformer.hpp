/*
 * MidiInputTransformer.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Brain/io.hpp"
#include <iostream>
#include <vector>

class MidiInputTransformer {
public:
  std::vector<ModelInput> parseMidiInput(MidiBuffer &buffer);
  ModelInput midiMessageToModelInput(const MidiMessage &message,
                                     int sampleNumber);
};
