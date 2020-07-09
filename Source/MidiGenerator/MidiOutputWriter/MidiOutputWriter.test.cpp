/*
 * MidiOutputWriter.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiOutputWriter.hpp"
#include <catch2/catch.hpp>

SCENARIO("MidiOutputWriter") {
  GIVEN("An instance of MidiOutputWriter") {
    MidiOutputWriter writer(5);

    REQUIRE(writer.getValue() == 4);
  }
}
