/*
 * MIDIOutputWriter.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Brain/io.hpp"
#include <vector>

/** Takes the output from the Model and converts it to the relevant MIDI
 * messages in the MIDIBuffer.
 *
 * It keeps track of what notes are playing and can be used to limit the length
 * of time that MIDI notes are allowed to play for.
 */
class MidiOutputWriter {
public:
  //============================================================================
  /** Default Constructor */
  MidiOutputWriter();
  /** Constructor to set initial maxNoteLength */
  MidiOutputWriter(uint_fast32_t maxNoteLength);

  //============================================================================
  /** Get the value of the global volume parameter. */
  float getGlobalVolume();

  /** Set the value of the global volume parameter. */
  void setGlobalVolume(float v);

  /** Get the minimum of the volume clip. */
  float getVolumeClipMin();

  /** Get the maximum of the volume clip. */
  float getVolumeClipMax();

  /** Set the minimum and maximum of the volume clip. */
  void setVolumeClip(float min, float max);

  /** Get the range of the volume clip. */
  float getVolumeClipRange();

  /** Write MIDI output to the buffer based on the Model's output.
   *
   * Given a sequence of output vectors from the Model, this writer creates MIDI
   * messages based on which notes are being activated. It also keeps track of
   * which notes are playing and can create NOTE_OFF messages if a note has been
   * playing longer than the maxiumum time.
   *
   * @param output The output vectors from the Model.
   * @param buffer The MIDI buffer to write to.
   * @param bufferSize The sample size of the current buffer.
   */
  MidiBuffer& writeMidiOutput(std::vector<ModelVector>& output, MidiBuffer& buffer, int bufferSize);

  /** Set the maximum length of a note in samples.
   * @param newMaxNoteLength The number of samples to clip the note length to.
   */
  void setMaxNoteLength(uint_fast32_t newMaxNoteLength);

  /** Stop clipping the length of time notes play for. */
  void disableMaxNoteLength();

private:
  //============================================================================
  /** A record of the MIDI notes that are playing, the array value is the number
   * of samples that the note has been playing for. */
  std::array<uint_fast32_t, NUM_MIDI_NOTES> playingMidiNotes{};

  /** Whether or not the writer should limit note length. */
  bool limitNoteLength;

  /** The maximum number of samples a note should play for. */
  uint_fast32_t maxNoteLength;

  /** The maximum value a value in the Network output should take. */
  float maxNetworkOutput;

  /** Value for the global MIDI volume slider. */
  float globalVolume;

  /** A pair of values to clip the MIDI volume between.
   *
   * This allows the quietest MIDI volumne and the loudest MIDI volume to be
   * set independently.
   */
  std::pair<float, float> volumeClip;

  //============================================================================
  /** Write a simgle ModelVector to the MidiBuffer
   *
   * @param vec A single output vector from the Model.
   * @param buffer The MIDI buffer to write to.
   * @param bufferSize The sample size of the current buffer.
   */
  void writeVectorToBuffer(ModelVector& vec, MidiBuffer& buffer, int bufferSize);

  /** Return whether the given note is playing.
   *
   * @param noteNumber The MIDI number of the note to check.
   */
  bool noteIsPlayingAt(int noteNumber);

  /** Returns whether or not a playing note needs to be stopped this buffer.
   *
   * @param noteNumber The MIDI number of the note.
   */
  bool noteNeedsEnding(int noteNumber);

  /** Add a note on event to the MIDI Buffer.
   *
   * This method performs no checks, it simply adds a note with the supplied
   * parameters, in the designated place in the buffer.
   *
   * @param buffer The MIDI Buffer to add the note to.
   * @param bufferSize The sample size of the current buffer.
   * @param vec Reference of the relevant ModelVector.
   * @param noteNumber The MIDI number of the note on event to add.
   */
  void addNoteOn(MidiBuffer& buffer, int bufferSize, ModelVector& vec, int noteNumber);

  /** Add a note off event to the MIDI Buffer.
   *
   * This method will look at the current notes playing to see if it actually
   * needs to end the note before the supplied sampleNumber. This could be the
   * case if the note happens to be ending in the same buffer that it is being
   * re-triggered.
   *
   * @param buffer The MIDI Buffer to add the note to.
   * @param noteNumber The MIDI number of the note off event to add.
   * @param sampleNumber When in the buffer to add the note.
   */
  void addNoteOff(MidiBuffer& buffer, int noteNumber, int sampleNumber);

  /** Get the sample number to send a note off MIDI message.
   *
   * This might be different from the sample number of the next note on event.
   * This will be the case if the note happens to be ending in the buffer that
   * the next note is triggered.
   *
   * @param vectorSampleNumber The sample number for the next note on event.
   * @param noteNumber The MIDI number of the note.
   * @param bufferSize The sample size of the current buffer.
   */
  int getEndSampleForNote(int vectorSampleNumber, int noteNumber, int bufferSize);

  /** Get the sample number to send an auto note off event.
   *
   * This method works out what sample number in the current buffer a note that
   * needs ending should end on, based on the maxNoteLength.
   *
   * @param noteNumber The MIDI number of the note.
   * @param bufferSize The sample size of the current buffer.
   */
  int getAutoEndSampleNumber(int noteNumber, int bufferSize);

  /** Set the current played samples value for a current playing MIDI note.
   *
   * This directly sets the number of samples in the playingMidiNotes memory.
   * This will likely only be called when a MIDI note is first played. The rest
   * of the time with value will simply be updated by the number of samples in a
   * buffer during each buffer.
   *
   * @param noteNumber The MIDI note number of the note to set.
   * @param numSamples The number of samples to set the current value to.
   */
  void setNotePlayingSamples(uint_least8_t noteNumber, uint_fast32_t numSamples);

  /** Add the buffer size to the number of samples of each playing note.
   *
   * This basically updates the value of each of the playing MIDI notes to the
   * number of samples that the note will have played by the end of the buffer.
   * This number can then be compared to the maxNoteLength to see if the note
   * needs to be stopped. This is done later, after the output vectors have been
   * processed, and any required MIDI events added to the buffer from that
   * process.
   *
   * @param bufferSize The sample size of the current buffer.
   */
  void updatePlayingNotesSamples(int bufferSize);

  /** Add note off events to the buffer for any notes that have played longer
   * than the maxNoteLength.
   *
   * @param buffer The MIDI buffer to write to.
   * @param bufferSize The sample size of the current buffer.
   */
  void stopNotesIfNeeded(MidiBuffer& buffer, int bufferSize);

  /** Ensure that the output of the Network is not great than some max.
   *
   * @param output The value of the Network output.
   */
  float clipBrainOutput(float output);
  /** Get the size of the output as a percentage of the max output.
   *
   * @param output The value of the network output.
   */
  float asPercentOfMax(float output);
  /** Get the MIDI velocity base on the current velocity clip.
   *
   * @param velocity The current velocity before the clip is applied.
   */
  float getClippedMidiVolume(float velocity);
  /** Get the MIDI velocity of a given Network output value.
   *
   * @param brainOutputValue The output value of the Network.
   */
  float getNoteVelocity(float brainOutputValue);
};

//==============================================================================
/** A function to reduce the ModelVector sample numbers modulo the buffer size.
 *
 * The ModelVector's hold an absolute sample number. When the vectors are
 * written to the MidiBuffer, the sample numbers used need to be relative to the
 * buffer, not the start of the time line. This is a convenience function to
 * reduce the sample numbers modulo the buffer size, before passing the result
 * to the MidiOutputWriter.
 *
 * @param output The output vectors of the model.
 * @param bufferSize The number of samples in the current buffer.
 */
void sampleNumModuloBufferSize(std::vector<ModelVector>& output, int bufferSize);
