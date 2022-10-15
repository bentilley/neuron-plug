/*
 * InputFilter.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "InputFilter.hpp"

/******************************
 * Constructors / Destructors *
 ******************************/

InputFilter::InputFilter()
    : mergeStrategy{MergeStrategy::SimpleMerge}, strategy{std::make_unique<SimpleMergeStrategy>()}
{}

InputFilter::InputFilter(MergeStrategy strat) : mergeStrategy{strat}
{
  switch (mergeStrategy) {
  default:
    strategy = std::make_unique<SimpleMergeStrategy>();
    break;
  }
}

/***********
 * Getters *
 ***********/

InputFilter::MergeStrategy InputFilter::getMergeStrategy() const { return mergeStrategy; }

/***********
 * Setters *
 ***********/

void InputFilter::setMergeStrategy(MergeStrategy newStrategy)
{
  mergeStrategy = newStrategy;
  switch (mergeStrategy) {
  default:
    strategy = std::make_unique<SimpleMergeStrategy>();
    break;
  }
}

/******************
 * Public Methods *
 ******************/

std::vector<ModelVector> InputFilter::mergeInputStreams(
  std::vector<std::vector<ModelVector>>& streams
) const
{
  std::vector<ModelVector> results;
  for (auto inputStream : streams) {
    for (auto inputVector : inputStream) {
      strategy->mergeInputVector(results, inputVector);
    }
  }
  return results;
}

/*************************
 * Simple Merge Strategy *
 *************************/

void SimpleMergeStrategy::mergeInputVector(
  std::vector<ModelVector>& mergedVectors,
  ModelVector& nextVector
)
{

  std::vector<ModelVector>::iterator vectorWithSameSampleNumber{mergedVectors.end()};
  for (auto v{mergedVectors.begin()}; v != mergedVectors.end(); ++v) {
    if (v->sampleNumber == nextVector.sampleNumber) {
      vectorWithSameSampleNumber = v;
      break;
    }
  }

  if (vectorWithSameSampleNumber != mergedVectors.end()) {
    *vectorWithSameSampleNumber = *vectorWithSameSampleNumber + nextVector;
  } else {
    nextVector.inputType = ModelVector::InputType::MergedInput;
    mergedVectors.push_back(nextVector);
  }
}
