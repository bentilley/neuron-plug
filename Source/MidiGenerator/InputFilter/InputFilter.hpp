/*
 * InputFilter.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../Brain/io.hpp"
#include <vector>

enum MergeStrategy : std::uint_least8_t {
  SimpleMerge,
  ForwardNeighbourMerge,
};

class InputFilter {
public:
  InputFilter();
  InputFilter(MergeStrategy strategy);

  std::vector<ModelInput>
  mergeInputStreams(std::vector<std::vector<ModelInput>> &streams);

  MergeStrategy getMergeStrategy();
  void setMergeStrategy(MergeStrategy newStrategy);

private:
  MergeStrategy mergeStrategy;
};
