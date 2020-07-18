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

std::vector<ModelInput>
MidiInputTransformer::getModelInputForBuffer(MidiBuffer &buffer) {
  return parseMidiInput(buffer);
}

std::vector<ModelInput>
MidiInputTransformer::parseMidiInput(MidiBuffer &buffer) {
  using InputMap = std::map<int, ModelInput>;
  InputMap inputBySample;
  int sampleNumber;
  MidiMessage message;
  for (MidiBuffer::Iterator i(buffer); i.getNextEvent(message, sampleNumber);) {
    if (message.isNoteOn()) {
      if (inputBySample.count(sampleNumber) == 1) {
        inputBySample.at(sampleNumber).data.at(message.getNoteNumber()) +=
            message.getFloatVelocity();
      } else {
        inputBySample.emplace(std::make_pair(
            sampleNumber, midiMessageToModelInput(message, sampleNumber)));
      }
    }
  }
  std::vector<ModelInput> transformedInput;
  for (InputMap::iterator it{inputBySample.begin()}; it != inputBySample.end();
       ++it) {
    transformedInput.push_back(it->second);
  }
  return transformedInput;
}

ModelInput
MidiInputTransformer::midiMessageToModelInput(const MidiMessage &message,
                                              int sampleNumber) {
  if (!message.isNoteOn()) {
    throw std::invalid_argument("MIDI message is not noteOn");
  }
  ModelInputVector data{};
  data.at(message.getNoteNumber()) = message.getFloatVelocity();
  return ModelInput{data, sampleNumber, ModelInputType::MidiInput};
}
