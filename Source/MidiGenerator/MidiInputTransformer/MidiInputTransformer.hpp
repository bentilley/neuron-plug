/*
 * MidiInputTransformer.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

class MidiInputTransformer {
public:
  MidiInputTransformer(int value);

  int getValue() { return value; }

private:
  int value{};
};
