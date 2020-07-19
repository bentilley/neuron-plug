/*
 * InputFilter.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "InputFilter.hpp"

InputFilter::InputFilter() : mergeStrategy{MergeStrategy::SimpleMerge} {}
InputFilter::InputFilter(MergeStrategy strategy) : mergeStrategy{strategy} {}

MergeStrategy InputFilter::getMergeStrategy() { return mergeStrategy; }

void InputFilter::setMergeStrategy(MergeStrategy strategy) {
  mergeStrategy = strategy;
}

std::vector<ModelInput>
InputFilter::mergeInputStreams(std::vector<std::vector<ModelInput>> &streams) {
  return std::vector<ModelInput>();
}
