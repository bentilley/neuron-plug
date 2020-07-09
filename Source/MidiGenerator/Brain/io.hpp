/*
 * io.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <array>
#include <vector>

constexpr int NUM_MIDI_NOTES = 128;

using ModelInputVector = std::array<float, NUM_MIDI_NOTES>;

enum ModelInputType : std::uint_least8_t {
  BeatClock,
  MidiInput,
};

struct ModelInput {
  ModelInput() = delete;
  ModelInput(ModelInputVector data, long sampleNum, ModelInputType type)
      : data{data}, sampleNumber{sampleNum}, inputType{type} {}

  ModelInputVector data;
  long sampleNumber;
  ModelInputType inputType;
};