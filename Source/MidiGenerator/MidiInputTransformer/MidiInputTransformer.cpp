/*
 * MidiInputTransformer.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiInputTransformer.hpp"

std::vector<ModelInput>
MidiInputTransformer::parseMidiInput(MidiBuffer &buffer) {
  using InputMap = std::map<int, ModelInput>;
  InputMap inputBySample;
  int sampleNumber;
  MidiMessage message;
  for (MidiBuffer::Iterator i(buffer); i.getNextEvent(message, sampleNumber);) {
    if (message.isNoteOn()) {
      if (inputBySample.find(sampleNumber) != inputBySample.end()) {
        inputBySample.at(sampleNumber).data.at(message.getNoteNumber()) +=
            message.getFloatVelocity();
      } else {
        inputBySample.at(sampleNumber) =
            midiMessageToModelInput(message, sampleNumber);
      }
    }
  }
  std::vector<ModelInput> transformedInput{};
  for (InputMap::iterator it{inputBySample.begin()}; it != inputBySample.end();
       ++it) {
    printf("adding somethign to output\n");
    transformedInput.push_back(it->second);
  }
  return transformedInput;
}

ModelInput
MidiInputTransformer::midiMessageToModelInput(const MidiMessage &message,
                                              int sampleNumber) {
  ModelInputVector data{};
  return ModelInput{data, sampleNumber, ModelInputType::MidiInput};
}
