/*
 * MidiGenerator.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiGenerator.hpp"

MidiGenerator::MidiGenerator(int num_neurons)
    : isOn{false}, receivesMidi{false}, beatClockIsOn(true), midiInputIsOn(true),
      network(num_neurons), midiInputTransformer(), inputFilter(), midiOutputWriter()
{}
MidiGenerator::~MidiGenerator() {}

/*
 * Getters & Setters
 */

void MidiGenerator::toggleOnOff() { isOn = !isOn; };

bool MidiGenerator::getIsOn() { return isOn; };

void MidiGenerator::toggleBeatClockIsOn() { beatClockIsOn = !beatClockIsOn; };

bool MidiGenerator::getBeatClockIsOn() { return beatClockIsOn; };

void MidiGenerator::setBeatClockInputWeight(double w) { beatClock.setWeight(w); };

double MidiGenerator::getBeatClockInputWeight() { return beatClock.getWeight(); };

int MidiGenerator::getBeatClockSubdivision() { return beatClock.getSubdivision(); }

void MidiGenerator::setBeatClockSubdivision(int s) { beatClock.setSubdivision(s); }

void MidiGenerator::toggleMidiInputIsOn() { midiInputIsOn = !midiInputIsOn; };

bool MidiGenerator::getMidiInputIsOn() { return midiInputIsOn; };

void MidiGenerator::setMidiInputInputWeight(double w) { midiInputTransformer.setWeight(w); };

double MidiGenerator::getMidiInputInputWeight() { return midiInputTransformer.getWeight(); };

void MidiGenerator::setMidiOutputWriterMaxNoteLength(int l)
{
  midiOutputWriter.setMaxNoteLength(l);
};

int MidiGenerator::getMidiOutputWriterMaxNoteLength()
{
  return midiOutputWriter.getMaxNoteLength();
};

float MidiGenerator::get_volume() { return midiOutputWriter.getGlobalVolume(); };

void MidiGenerator::set_volume(float v) { midiOutputWriter.setGlobalVolume(v); };

float MidiGenerator::get_volume_clip_min() { return midiOutputWriter.getVolumeClipMin(); };

float MidiGenerator::get_volume_clip_max() { return midiOutputWriter.getVolumeClipMax(); };

void MidiGenerator::set_volume_clip(float min, float max)
{
  midiOutputWriter.setVolumeClip(min, max);
};

// MIDI Notes
int MidiGenerator::get_neuron_midi_note(int neuron_idx)
{
  return network.getNeuronMidiNote(neuron_idx);
}

void MidiGenerator::set_neuron_midi_note(int neuron_idx, int new_note_number)
{
  network.setNeuronMidiNote(neuron_idx, new_note_number);
}

// Input Weight
int MidiGenerator::get_neuron_input_weight(int neuron_idx)
{
  PluginLogger::logger.logVec<int>("input weights", network.getInputWeights());
  return network.getInputWeightForNeuron(neuron_idx);
}

void MidiGenerator::set_neuron_input_weight(int neuron_idx, int new_input_weight)
{
  network.setInputWeightForNeuron(neuron_idx, new_input_weight);
  PluginLogger::logger.logVec<int>("input weights", network.getInputWeights());
}

// Threshold
int MidiGenerator::get_neuron_threshold(int neuron_idx)
{
  return network.getThresholdForNeuron(neuron_idx);
}

void MidiGenerator::set_neuron_threshold(int neuron_idx, int new_threshold)
{
  network.setThresholdForNeuron(neuron_idx, new_threshold);
  std::vector<int> thresholds(num_neurons(), 0);
  std::vector<Neuron> neurons = network.getNeurons();
  std::transform(neurons.begin(), neurons.end(), thresholds.begin(), [](Neuron n) {
    return n.getThreshold();
  });
  PluginLogger::logger.logVec<int>("thresholds", thresholds);
}

// Connection Weights
int MidiGenerator::get_neuron_connection_weight(int from, int to)
{
  return network.getConnectionWeightForNeurons(from, to);
}

void MidiGenerator::set_neuron_connection_weight(int from, int to, int new_connection_weight)
{
  network.setConnectionWeightForNeurons(from, to, new_connection_weight);
  PluginLogger::logger.logVec<int>(
    "Connection weights from " + String(from),
    network.getConnectionWeights().at(from)
  );
}

/*
 * Neuron Model Methods
 */

int MidiGenerator::num_neurons() { return network.numNeurons(); };

void MidiGenerator::add_neuron() { network.addNeuron(); }

void MidiGenerator::remove_neuron() { network.removeNeuron(); }

void MidiGenerator::remove_neuron_at(int index) { network.removeNeuronAt(index); }

/*
 * Audio Thread
 */

void MidiGenerator::generate_next_midi_buffer(
  MidiBuffer& midiInput,
  MidiBuffer& midiOutput,
  const AudioPlayHead::CurrentPositionInfo& pos,
  SystemInfo& sys
)
{
  std::vector<std::vector<ModelVector>> inputs;

  if (beatClockIsOn) {
    inputs.push_back(beatClock.getModelInputForBuffer(pos, sys));
  }

  if (midiInputIsOn) {
    inputs.push_back(midiInputTransformer.getModelInputForBuffer(midiInput));
  }

  auto input = inputFilter.mergeInputStreams(inputs);

  std::vector<ModelVector> output;
  output.reserve(input.size());

  for (auto it = input.begin(); it != input.end(); ++it) {
    output.emplace_back(network.processNext(*it));
  }

  midiOutputWriter.writeMidiOutput(output, midiOutput, sys.numBufferSamples);
};
