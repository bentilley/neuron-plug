/*
 * BeatClock.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Brain/io.hpp"

typedef AudioPlayHead::CurrentPositionInfo PositionInfo;

struct SystemInfo {
  SystemInfo() = delete;
  SystemInfo(double sampleRate, int numBufferSamples)
      : sampleRate{sampleRate}, numBufferSamples{numBufferSamples} {}

  double sampleRate;
  int numBufferSamples;
};

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
  BeatClock(int subdivision, double outputScaleFactor);

  int getSubdivision();
  void setSubdivision(int new_subdivision);

  double getOutputScaleFactor();
  void setOutputScaleFactor(double newScaleFactor);

  std::vector<ModelInput> getModelInputForBuffer(PositionInfo &positionInfo,
                                                 SystemInfo &systemInfo);

private:
  int subdivision;
  double outputScaleFactor;

  float getSamplesPerSubdivision(float bpm, double sampleRate);
  int64_t getNumberOfNextHit(float bpm, int64_t timeInSamples,
                             double sampleRate);
};
