/*
 * InputFilter.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "InputFilter.hpp"
#include <catch2/catch.hpp>

SCENARIO("InputFilter") {
  GIVEN("An instance of InputFilter") {
    InputFilter filter(5);

    REQUIRE(filter.getValue() == 4);
  }
}
