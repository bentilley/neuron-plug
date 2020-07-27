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

using ModelVectorData = std::array<float, NUM_MIDI_NOTES>;

struct ModelVector {

  enum InputType : std::uint_least8_t {
    BeatClockInput,
    MidiInput,
    MergedInput,
  };

  ModelVector() = delete;
  ModelVector(ModelVectorData data, int32_t sampleNum, InputType type)
      : data{data}, sampleNumber{sampleNum}, inputType{type}
  {
  }
  ModelVector(float dataInitValue, int32_t sampleNum, InputType type)
      : sampleNumber{sampleNum}, inputType{type}
  {
    data.fill(dataInitValue);
  }

  friend bool operator==(const ModelVector &m1, const ModelVector &m2)
  {
    return m1.data == m2.data && m1.sampleNumber == m2.sampleNumber &&
           m1.inputType == m2.inputType;
  }

  friend ModelVector operator+(const ModelVector &m1, const ModelVector &m2)
  {
    ModelVectorData mergedData;
    for (int i{0}; i < mergedData.size(); ++i) {
      mergedData.at(i) = m1.data.at(i) + m2.data.at(i);
    }
    return ModelVector(mergedData, m1.sampleNumber, InputType::MergedInput);
  }

  ModelVectorData data;
  int32_t sampleNumber;
  InputType inputType;
};
