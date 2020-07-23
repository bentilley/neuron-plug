/*
 * InputFilter.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "../Brain/io.hpp"
#include <catch2/catch.hpp>
#include <vector>

#define private public
#include "InputFilter.hpp"

SCENARIO("InputFilter - SimpleMerge Strategy") {
  GIVEN("An instance of InputFilter with the SimpleMerge strategy") {
    InputFilter filter{InputFilter::MergeStrategy::SimpleMerge};

    GIVEN("an empty array of input vectors") {
      std::vector<std::vector<ModelInput>> inputs;

      THEN("we merge the input") {
        std::vector<ModelInput> result{filter.mergeInputStreams(inputs)};

        REQUIRE(result.size() == 0);
      }
    }

    GIVEN("one input stream with one input vector") {
      ModelInput input1{0.5f, 100, ModelInputType::BeatClockInput};
      std::vector<std::vector<ModelInput>> inputs{
          std::vector<ModelInput>{input1}};

      THEN("we merge the input") {
        std::vector<ModelInput> result{filter.mergeInputStreams(inputs)};

        REQUIRE(result.size() == 1);
        input1.inputType = ModelInputType::MergedInput;
        REQUIRE(result.at(0) == input1);
      }
    }

    GIVEN("one input stream with multiple input vectors") {
      ModelInput input1{0.5f, 100, ModelInputType::BeatClockInput};
      ModelInput input2{0.4f, 200, ModelInputType::BeatClockInput};
      std::vector<std::vector<ModelInput>> inputs{
          std::vector<ModelInput>{input1, input2}};

      THEN("we merge the input") {
        std::vector<ModelInput> result{filter.mergeInputStreams(inputs)};

        REQUIRE(result.size() == 2);
        input1.inputType = ModelInputType::MergedInput;
        REQUIRE(result.at(0) == input1);
        input2.inputType = ModelInputType::MergedInput;
        REQUIRE(result.at(1) == input2);
      }
    }

    GIVEN("two input stream no vector collision") {
      ModelInput input1{0.5f, 100, ModelInputType::BeatClockInput};
      ModelInput input2{0.4f, 200, ModelInputType::MidiInput};
      std::vector<std::vector<ModelInput>> inputs{
          std::vector<ModelInput>{input1},
          std::vector<ModelInput>{input2}};

      THEN("we merge the input") {
        std::vector<ModelInput> result{filter.mergeInputStreams(inputs)};

        REQUIRE(result.size() == 2);
        input1.inputType = ModelInputType::MergedInput;
        REQUIRE(result.at(0) == input1);
        input2.inputType = ModelInputType::MergedInput;
        REQUIRE(result.at(1) == input2);
      }
    }

    GIVEN("two input stream with vector collision") {
      ModelInput input1{0.5f, 100, ModelInputType::BeatClockInput};
      ModelInput input2{0.4f, 100, ModelInputType::MidiInput};
      std::vector<std::vector<ModelInput>> inputs{
          std::vector<ModelInput>{input1},
          std::vector<ModelInput>{input2}};

      THEN("we merge the input") {
        std::vector<ModelInput> result{filter.mergeInputStreams(inputs)};

        REQUIRE(result.size() == 1);
        REQUIRE(result.at(0) == input1 + input2);
      }
    }
  }
}
