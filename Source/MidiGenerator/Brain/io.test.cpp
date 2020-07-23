/*
 * io.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "io.hpp"
#include <catch2/catch.hpp>

SCENARIO("Brain I/O") {
  GIVEN("ModelInput for a Beat Clock") {
    ModelInputVector data{};
    ModelInput input{data, 17362, ModelInputType::BeatClockInput};

    REQUIRE(input.data.size() == 128);
    for (auto element : input.data) {
      REQUIRE(element == 0);
    }
    REQUIRE(input.sampleNumber == 17362);
    REQUIRE(input.inputType == ModelInputType::BeatClockInput);
  }

  GIVEN("ModelInput for a MIDI Input Transformer") {
    ModelInputVector data{};
    data.at(13) = 0.54f;
    data.at(89) = 0.972f;
    ModelInput input{data, 9827643, ModelInputType::MidiInput};

    REQUIRE(input.data.size() == 128);
    REQUIRE(input.data.at(13) == Approx(0.54f));
    REQUIRE(input.data.at(89) == Approx(0.972f));
    REQUIRE(input.sampleNumber == 9827643);
    REQUIRE(input.inputType == ModelInputType::MidiInput);
  }

  GIVEN("ModelInput for a MIDI Input Transformer and a Beat Clock") {
    ModelInputVector data{};
    data.at(13) = 0.54f;
    data.at(89) = 0.972f;
    ModelInput input_midi{data, 9827643, ModelInputType::MidiInput};
    ModelInput input_clock{0.5f, 9827643, ModelInputType::BeatClockInput};

    auto result = input_midi + input_clock;

    REQUIRE(result.data.size() == 128);
    REQUIRE(result.data.at(13) == Approx(1.04f));
    REQUIRE(result.data.at(89) == Approx(1.472f));
    for (int i{0}; i < result.data.size(); ++i) {
      if (i != 13 && i != 89) {
        REQUIRE(result.data.at(i) == Approx(0.5f));
      }
    }
    REQUIRE(result.sampleNumber == 9827643);
    REQUIRE(result.inputType == ModelInputType::MergedInput);
  }
}
