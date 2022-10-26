/*
 * BeatClock.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "BeatClock.hpp"
#include <cmath>

BeatClock::BeatClock() : subdivision{1}, weight{1.0} {};

BeatClock::BeatClock(int subdivision, double weight) : subdivision{subdivision}, weight{weight} {}

int BeatClock::getSubdivision() { return subdivision; }

void BeatClock::setSubdivision(int new_subdiv) { subdivision = new_subdiv; }

double BeatClock::getWeight() { return weight; }

void BeatClock::setWeight(double newWeight) { weight = newWeight; }

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
    result.emplace_back(weight, sampleNumber, ModelVector::InputType::BeatClockInput);
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
