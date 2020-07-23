/*
 * MidiInputTransformer.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiInputTransformer.hpp"
#include <map>
#include <stdexcept>
#include <utility>

std::vector<ModelVector>
MidiInputTransformer::getModelInputForBuffer(MidiBuffer &buffer) {
  return parseMidiInput(buffer);
}

std::vector<ModelVector>
MidiInputTransformer::parseMidiInput(MidiBuffer &buffer) {
  using InputMap = std::map<int, ModelVector>;
  InputMap inputBySample;
  int sampleNumber;
  MidiMessage message;
  for (MidiBuffer::Iterator i(buffer); i.getNextEvent(message, sampleNumber);) {
    if (message.isNoteOn()) {
      if (inputBySample.count(sampleNumber) == 1) {
        inputBySample.at(sampleNumber).data.at(message.getNoteNumber()) +=
            message.getFloatVelocity();
      } else {
        inputBySample.emplace(
            std::make_pair(sampleNumber,
                           midiMessageToModelInput(message, sampleNumber)));
      }
    }
  }
  std::vector<ModelVector> transformedInput;
  for (InputMap::iterator it{inputBySample.begin()}; it != inputBySample.end();
       ++it) {
    transformedInput.push_back(it->second);
  }
  return transformedInput;
}

ModelVector
MidiInputTransformer::midiMessageToModelInput(const MidiMessage &message,
                                              int sampleNumber) {
  if (!message.isNoteOn()) {
    throw std::invalid_argument("MIDI message is not noteOn");
  }
  ModelVectorData data{};
  data.at(message.getNoteNumber()) = message.getFloatVelocity();
  return ModelVector{data,
                     (int64_t)sampleNumber,
                     ModelVector::InputType::MidiInput};
}
