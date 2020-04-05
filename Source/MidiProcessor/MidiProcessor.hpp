/*
 * MidiProcessor.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include <vector>

class MidiProcessor {
public:
  MidiProcessor();
  ~MidiProcessor();

  bool test();
  bool testFail();

  void render_buffer(MidiBuffer &buffer, std::vector<int> next_output);

private:
};
