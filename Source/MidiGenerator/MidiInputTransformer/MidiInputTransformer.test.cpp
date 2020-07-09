/*
 * MidiInputTransformer.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiInputTransformer.hpp"
#include <catch2/catch.hpp>

SCENARIO("MidiInputTransformer") {
  GIVEN("An instance of MidiInputTransformer") {
    MidiInputTransformer transformer(5);

    REQUIRE(transformer.getValue() == 4);
  }
}
