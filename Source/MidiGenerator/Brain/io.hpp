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
  BeatClockInput,
  MidiInput,
  MergedInput,
};

struct ModelInput {
  ModelInput() = delete;
  ModelInput(ModelInputVector data, int64_t sampleNum, ModelInputType type)
      : data{data}, sampleNumber{sampleNum}, inputType{type} {}
  ModelInput(float dataInitValue, int64_t sampleNum, ModelInputType type)
      : sampleNumber{sampleNum}, inputType{type} {
    data.fill(dataInitValue);
  }

  friend bool operator==(const ModelInput &m1, const ModelInput &m2) {
    return m1.data == m2.data && m1.sampleNumber == m2.sampleNumber &&
           m1.inputType == m2.inputType;
  }

  friend ModelInput operator+(const ModelInput &m1, const ModelInput &m2) {
    ModelInputVector mergedData;
    for (int i{0}; i < mergedData.size(); ++i) {
      mergedData.at(i) = m1.data.at(i) + m2.data.at(i);
    }
    return ModelInput(mergedData, m1.sampleNumber, ModelInputType::MergedInput);
  }

  ModelInputVector data;
  int64_t sampleNumber;
  ModelInputType inputType;
};
