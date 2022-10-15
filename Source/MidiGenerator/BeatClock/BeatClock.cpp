/*
 * BeatClock.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "BeatClock.hpp"
#include <cmath>

BeatClock::BeatClock() : subdivision{1}, modelInputScaleFactor{1.0} {};

BeatClock::BeatClock(int subdivision, double modelInputScaleFactor)
    : subdivision{subdivision}, modelInputScaleFactor{modelInputScaleFactor}
{}

/*
 * Getters
 */

int BeatClock::getSubdivision() { return subdivision; }

double BeatClock::getModelInputScaleFactor() { return modelInputScaleFactor; }

/*
 * Setters
 */

void BeatClock::setSubdivision(int new_subdiv) { subdivision = new_subdiv; }

void BeatClock::setModelInputScaleFactor(double newScaleFactor)
{
  modelInputScaleFactor = newScaleFactor;
}

/*
 * Public Methods
 */

std::vector<ModelVector> BeatClock::getModelInputForBuffer(const PositionInfo& pos, SystemInfo& sys)
{
  std::vector<ModelVector> result;
  float samplesPerSubdivision = getSamplesPerSubdivision(pos.bpm, sys.sampleRate);
  int64_t numberOfNextHit = getNumberOfNextHit(pos.bpm, pos.timeInSamples, sys.sampleRate);
  int64_t lastSampleOfBuffer = pos.timeInSamples + sys.numBufferSamples;

  while (numberOfNextHit * samplesPerSubdivision <= lastSampleOfBuffer) {
    int64_t sampleNumber = std::round(numberOfNextHit * samplesPerSubdivision);
    result
      .emplace_back(modelInputScaleFactor, sampleNumber, ModelVector::InputType::BeatClockInput);
    ++numberOfNextHit;
  }

  return result;
}

/*
 * Private Methods
 */

float BeatClock::getSamplesPerSubdivision(float bpm, double sampleRate)
{
  return ((60.0 / bpm) / (float)subdivision) * sampleRate;
}

int64_t BeatClock::getNumberOfNextHit(float bpm, int64_t timeInSamples, double sampleRate)
{
  return std::ceil(timeInSamples / getSamplesPerSubdivision(bpm, sampleRate));
}
