/*
 * InputFilter.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "../Brain/io.hpp"
#include <catch2/catch.hpp>
#include <vector>

#include "InputFilter.hpp"

SCENARIO("InputFilter - SimpleMerge Strategy") {
  GIVEN("An instance of InputFilter with the SimpleMerge strategy") {
    InputFilter filter{InputFilter::MergeStrategy::SimpleMerge};

    GIVEN("an empty array of input vectors") {
      std::vector<std::vector<ModelVector>> inputs;

      THEN("we merge the input") {
        std::vector<ModelVector> result{filter.mergeInputStreams(inputs)};

        REQUIRE(result.size() == 0);
      }
    }

    GIVEN("one input stream with one input vector") {
      ModelVector input1{0.5f, 100, ModelVector::InputType::BeatClockInput};
      std::vector<std::vector<ModelVector>> inputs{
          std::vector<ModelVector>{input1}};

      THEN("we merge the input") {
        std::vector<ModelVector> result{filter.mergeInputStreams(inputs)};

        REQUIRE(result.size() == 1);
        input1.inputType = ModelVector::InputType::MergedInput;
        REQUIRE(result.at(0) == input1);
      }
    }

    GIVEN("one input stream with multiple input vectors") {
      ModelVector input1{0.5f, 100, ModelVector::InputType::BeatClockInput};
      ModelVector input2{0.4f, 200, ModelVector::InputType::BeatClockInput};
      std::vector<std::vector<ModelVector>> inputs{
          std::vector<ModelVector>{input1, input2}};

      THEN("we merge the input") {
        std::vector<ModelVector> result{filter.mergeInputStreams(inputs)};

        REQUIRE(result.size() == 2);
        input1.inputType = ModelVector::InputType::MergedInput;
        REQUIRE(result.at(0) == input1);
        input2.inputType = ModelVector::InputType::MergedInput;
        REQUIRE(result.at(1) == input2);
      }
    }

    GIVEN("two input stream no vector collision") {
      ModelVector input1{0.5f, 100, ModelVector::InputType::BeatClockInput};
      ModelVector input2{0.4f, 200, ModelVector::InputType::MidiInput};
      std::vector<std::vector<ModelVector>> inputs{
          std::vector<ModelVector>{input1},
          std::vector<ModelVector>{input2}};

      THEN("we merge the input") {
        std::vector<ModelVector> result{filter.mergeInputStreams(inputs)};

        REQUIRE(result.size() == 2);
        input1.inputType = ModelVector::InputType::MergedInput;
        REQUIRE(result.at(0) == input1);
        input2.inputType = ModelVector::InputType::MergedInput;
        REQUIRE(result.at(1) == input2);
      }
    }

    GIVEN("two input stream with vector collision") {
      ModelVector input1{0.5f, 100, ModelVector::InputType::BeatClockInput};
      ModelVector input2{0.4f, 100, ModelVector::InputType::MidiInput};
      std::vector<std::vector<ModelVector>> inputs{
          std::vector<ModelVector>{input1},
          std::vector<ModelVector>{input2}};

      THEN("we merge the input") {
        std::vector<ModelVector> result{filter.mergeInputStreams(inputs)};

        REQUIRE(result.size() == 1);
        REQUIRE(result.at(0) == input1 + input2);
      }
    }
  }
}
