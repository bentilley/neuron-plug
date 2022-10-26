/*
 * MidiOutputWriter.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiOutputWriter.hpp"
#include <cassert>

/****************
 * Constructors *
 ****************/

MidiOutputWriter::MidiOutputWriter()
    : maxNoteLength{0}, maxNetworkOutput{1.0}, globalVolume{1.0}, volumeClip(0.0, 1.0)
{}

MidiOutputWriter::MidiOutputWriter(uint_fast32_t maxNoteLength)
    : maxNoteLength{maxNoteLength}, maxNetworkOutput{1.0}, globalVolume{1.0}, volumeClip(0.0, 1.0)
{}

/******************
 * Public Methods *
 ******************/

MidiBuffer& MidiOutputWriter::writeMidiOutput(
  std::vector<ModelVector>& output,
  MidiBuffer& buffer,
  int bufferSize
)
{
  updatePlayingNotesSamples(bufferSize);
  sampleNumModuloBufferSize(output, bufferSize);
  for (auto vec : output) {
    writeVectorToBuffer(vec, buffer, bufferSize);
  }
  stopNotesIfNeeded(buffer, bufferSize);
  return buffer;
}

/***********************
 * Getters and Setters *
 ***********************/

float MidiOutputWriter::getGlobalVolume() { return globalVolume; };

void MidiOutputWriter::setGlobalVolume(float v) { globalVolume = v; };

float MidiOutputWriter::getVolumeClipMin() { return volumeClip.first; };

float MidiOutputWriter::getVolumeClipMax() { return volumeClip.second; };

void MidiOutputWriter::setVolumeClip(float min, float max)
{
  assert(min <= max && max <= 1.0);
  volumeClip = std::make_pair(min, max);
};

float MidiOutputWriter::getVolumeClipRange() { return volumeClip.second - volumeClip.first; }

uint_fast32_t MidiOutputWriter::getMaxNoteLength() { return maxNoteLength; }

void MidiOutputWriter::setMaxNoteLength(uint_fast32_t newMaxNoteLength)
{
  maxNoteLength = newMaxNoteLength;
}

void MidiOutputWriter::disableMaxNoteLength() { maxNoteLength = 0; }

/*******************
 * Private Methods *
 *******************/

void MidiOutputWriter::writeVectorToBuffer(ModelVector& vec, MidiBuffer& buffer, int bufferSize)
{
  for (int noteNumber{0}; noteNumber < static_cast<int>(vec.data.size()); ++noteNumber) {
    if (vec.data.at(noteNumber) > 0.001) {
      if (!noteIsPlayingAt(noteNumber)) {
        addNoteOn(buffer, bufferSize, vec, noteNumber);
      } else {
        int endingSampleNumber = getEndSampleForNote(vec.sampleNumber, noteNumber, bufferSize);
        addNoteOff(buffer, noteNumber, endingSampleNumber);
        addNoteOn(buffer, bufferSize, vec, noteNumber);
      }
    }
  }
}

bool MidiOutputWriter::noteIsPlayingAt(int noteNumber)
{
  return playingMidiNotes.at(noteNumber) > 0;
}

bool MidiOutputWriter::noteNeedsEnding(int noteNumber)
{
  if (maxNoteLength == 0)
    return false;

  return playingMidiNotes.at(noteNumber) > maxNoteLength;
}

void MidiOutputWriter::addNoteOn(
  MidiBuffer& buffer,
  int bufferSize,
  ModelVector& vec,
  int noteNumber
)
{
  float vel = getNoteVelocity(vec.data.at(noteNumber));
  buffer.addEvent(MidiMessage::noteOn(1, noteNumber, vel), vec.sampleNumber);
  setNotePlayingSamples(noteNumber, bufferSize - vec.sampleNumber - 1);
}

void MidiOutputWriter::addNoteOff(MidiBuffer& buffer, int noteNumber, int sampleNumber)
{
  buffer.addEvent(MidiMessage::noteOff(1, noteNumber, 1.0f), sampleNumber);
  setNotePlayingSamples(noteNumber, 0);
}

int MidiOutputWriter::getEndSampleForNote(int vectorSampleNumber, int noteNumber, int bufferSize)
{
  if (!noteNeedsEnding(noteNumber)) {
    return vectorSampleNumber;
  } else {
    int endingSampleNumber = getAutoEndSampleNumber(noteNumber, bufferSize);
    return std::min(endingSampleNumber, vectorSampleNumber);
  }
}

int MidiOutputWriter::getAutoEndSampleNumber(int noteNumber, int bufferSize)
{
  int currentNoteLength = playingMidiNotes.at(noteNumber);
  assert(currentNoteLength >= static_cast<int>(maxNoteLength));

  return bufferSize - 1 - (currentNoteLength - maxNoteLength);
}

void MidiOutputWriter::setNotePlayingSamples(uint_least8_t noteNumber, uint_fast32_t numSamples)
{
  playingMidiNotes.at(noteNumber) = numSamples;
}

void MidiOutputWriter::updatePlayingNotesSamples(int bufferSize)
{
  for (int noteNumber{0}; noteNumber < static_cast<int>(playingMidiNotes.size()); ++noteNumber) {
    if (playingMidiNotes.at(noteNumber) > 0) {
      playingMidiNotes.at(noteNumber) += bufferSize;
    }
  }
}

void MidiOutputWriter::stopNotesIfNeeded(MidiBuffer& buffer, int bufferSize)
{
  if (maxNoteLength == 0)
    return;

  for (int noteNumber{0}; noteNumber < static_cast<int>(playingMidiNotes.size()); ++noteNumber) {
    if (playingMidiNotes.at(noteNumber) > maxNoteLength) {
      addNoteOff(buffer, noteNumber, getAutoEndSampleNumber(noteNumber, bufferSize));
    }
  }
}

float MidiOutputWriter::clipBrainOutput(float output)
{
  return output <= maxNetworkOutput ? output : maxNetworkOutput;
}

float MidiOutputWriter::asPercentOfMax(float output) { return output / maxNetworkOutput; }

float MidiOutputWriter::getClippedMidiVolume(float vel)
{
  if (vel < 0.001)
    return 0;
  return volumeClip.first + (getVolumeClipRange() * vel);
}

float MidiOutputWriter::getNoteVelocity(float brainOutputValue)
{
  float output = asPercentOfMax(clipBrainOutput(brainOutputValue));
  return getClippedMidiVolume(output) * globalVolume;
}

/*******************
 * Other Functions *
 *******************/

/***TODO Do we need sampleNumModuloBufferSize method?
 *
 * I think that the ModelVector's coming from the MidiInputTransformer are
 * already mod the bufferSize, so if the same is true for the BeatClock, then
 * this method isn't needed. Then we just need to enforce the standard for input
 * generating devices.
 */
void sampleNumModuloBufferSize(std::vector<ModelVector>& output, int bufferSize)
{
  for (auto& modelVector : output) {
    modelVector.sampleNumber = modelVector.sampleNumber % bufferSize;
  }
}
