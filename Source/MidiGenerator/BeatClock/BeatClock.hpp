/*
 * BeatClock.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Brain/io.hpp"
#include "../transport.hpp"

typedef AudioPlayHead::CurrentPositionInfo PositionInfo;

/** Generate input for the plugin Brain based on the current sample number.
 *
 * The Beatclock is used for the plugin to know when to automatically perform
 * iterations of the Brain's algorithm. The subdivision parameter can be used to
 * set how many times per `beat` the clock fires input into the Brain. (e.g. a
 * subdivision of `4` will mean that the Brain receives input from the clock 4
 * times per beat - i.e. semi-quavers).
 */
class BeatClock {
public:
  BeatClock();
  BeatClock(int subdivision, double modelInputScaleFactor);

  /** Get the #subdivision from this BeatClock. */
  int getSubdivision();
  /** Set the #subdivision from this BeatClock. */
  void setSubdivision(int new_subdivision);

  /** Get the #modelInputScaleFactor from this BeatClock. */
  double getModelInputScaleFactor();
  /** Set the #modelInputScaleFactor from this BeatClock. */
  void setModelInputScaleFactor(double newScaleFactor);

  /** Get the Brain input vectors for this buffer.
   *
   * This is the main method of the BeatClock. It is called each buffer to get
   * the input vectors for the Brain.
   *
   * @param positionInfo The JUCE CurrentPositionInfo for the buffer.
   * @param systemInfo Information about the current DAW settings.
   * @returns The input vectors for the plugin model.
   */
  std::vector<ModelVector> getModelInputForBuffer(
    const PositionInfo& positionInfo,
    SystemInfo& systemInfo
  );

private:
  int subdivision;              /**< The number of times the Clock fires each beat. */
  double modelInputScaleFactor; /**< The value of the input vectors generated. */

  /** Calculate the number of samples per subdivision.
   *
   * @param bpm The current beats per minute of the audio host.
   * @param sampleRate The current sample rate of the audio host.
   * @returns The number of samples per subdivision.
   */
  float getSamplesPerSubdivision(float bpm, double sampleRate);

  /** Calculate the number of the next time the BeatClock will fire.
   *
   * Given the current sample number, this calculates how many times the
   * BeatClock will / would have fired up until that point plus one, i.e. if the
   * clock would have fired `N` times upto sample number `x`, this method would
   * return `N+1`.
   *
   * @param bpm The current beats per minute of the audio host.
   * @param timeInSamples The sample number of the first sample of the buffer.
   * @param sampleRate The current sample rate of the audio host.
   * @returns The number of the next hit.
   */
  int64_t getNumberOfNextHit(float bpm, int64_t timeInSamples, double sampleRate);
};
