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
