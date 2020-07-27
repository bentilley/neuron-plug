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

MidiOutputWriter::MidiOutputWriter() : limitNoteLength{false}, maxNoteLength{0}
{
}

MidiOutputWriter::MidiOutputWriter(uint_fast32_t maxNoteLength)
    : limitNoteLength{true}, maxNoteLength{maxNoteLength}
{
}

/******************
 * Public Methods *
 ******************/

MidiBuffer &MidiOutputWriter::writeMidiOutput(
  std::vector<ModelVector> &output,
  MidiBuffer &buffer,
  int bufferSize)
{
  updatePlayingNotesSamples(bufferSize);
  sampleNumModuloBufferSize(output, bufferSize);
  for (auto vec : output) {
    writeVectorToBuffer(vec, buffer, bufferSize);
  }
  stopNotesIfNeeded(buffer, bufferSize);
  return buffer;
}

/***********
 * Setters *
 ***********/

void MidiOutputWriter::setMaxNoteLength(uint_fast32_t newMaxNoteLength)
{
  maxNoteLength = newMaxNoteLength;
  limitNoteLength = true;
}

void MidiOutputWriter::disableMaxNoteLength()
{
  limitNoteLength = false;
  maxNoteLength = 0;
}

/*******************
 * Private Methods *
 *******************/

void MidiOutputWriter::writeVectorToBuffer(
  ModelVector &vec,
  MidiBuffer &buffer,
  int bufferSize)
{
  for (int noteNumber{0}; noteNumber < vec.data.size(); ++noteNumber) {
    if (vec.data.at(noteNumber) > 0.001) {
      if (!noteIsPlayingAt(noteNumber)) {
        addNoteOn(buffer, bufferSize, vec, noteNumber);
      } else {
        int endingSampleNumber =
          getEndSampleForNote(vec.sampleNumber, noteNumber, bufferSize);
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
  if (!limitNoteLength)
    return false;

  return playingMidiNotes.at(noteNumber) > maxNoteLength;
}

void MidiOutputWriter::addNoteOn(
  MidiBuffer &buffer,
  int bufferSize,
  ModelVector &vec,
  int noteNumber)
{
  buffer.addEvent(
    MidiMessage::noteOn(1, noteNumber, vec.data.at(noteNumber)),
    vec.sampleNumber);
  setNotePlayingSamples(noteNumber, bufferSize - vec.sampleNumber - 1);
}

void MidiOutputWriter::addNoteOff(
  MidiBuffer &buffer,
  int noteNumber,
  int sampleNumber)
{
  buffer.addEvent(MidiMessage::noteOff(1, noteNumber, 1.0f), sampleNumber);
  setNotePlayingSamples(noteNumber, 0);
}

int MidiOutputWriter::getEndSampleForNote(
  int vectorSampleNumber,
  int noteNumber,
  int bufferSize)
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
  assert(currentNoteLength >= maxNoteLength);

  return bufferSize - 1 - (currentNoteLength - maxNoteLength);
}

void MidiOutputWriter::setNotePlayingSamples(
  uint_least8_t noteNumber,
  uint_fast32_t numSamples)
{
  playingMidiNotes.at(noteNumber) = numSamples;
}

void MidiOutputWriter::updatePlayingNotesSamples(int bufferSize)
{
  for (int noteNumber{0}; noteNumber < playingMidiNotes.size(); ++noteNumber) {
    if (playingMidiNotes.at(noteNumber) > 0) {
      playingMidiNotes.at(noteNumber) += bufferSize;
    }
  }
}

void MidiOutputWriter::stopNotesIfNeeded(MidiBuffer &buffer, int bufferSize)
{
  if (!limitNoteLength)
    return;

  for (int noteNumber{0}; noteNumber < playingMidiNotes.size(); ++noteNumber) {
    if (playingMidiNotes.at(noteNumber) > maxNoteLength) {
      addNoteOff(
        buffer,
        noteNumber,
        getAutoEndSampleNumber(noteNumber, bufferSize));
    }
  }
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
void sampleNumModuloBufferSize(std::vector<ModelVector> &output, int bufferSize)
{
  for (auto &modelVector : output) {
    modelVector.sampleNumber = modelVector.sampleNumber % bufferSize;
  }
}
