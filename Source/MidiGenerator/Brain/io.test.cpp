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
    ModelInput input{data, 17362, ModelInputType::BeatClock};

    REQUIRE(input.data.size() == 128);
    for (auto element : input.data) {
      REQUIRE(element == 0);
    }
    REQUIRE(input.sampleNumber == 17362);
    REQUIRE(input.inputType == ModelInputType::BeatClock);
  }

  GIVEN("ModelInput for a MIDI Input Transformer") {
    ModelInputVector data{};
    data.at(13) = 0.54f;
    data.at(89) = 0.972f;
    ModelInput input{data, 9827643, ModelInputType::MidiInput};

    REQUIRE(input.data.size() == 128);
    REQUIRE(input.data.at(13) == Approx(0.54f));
    REQUIRE(input.data.at(89) == Approx(0.972));
    REQUIRE(input.sampleNumber == 9827643);
    REQUIRE(input.inputType == ModelInputType::MidiInput);
  }
}
