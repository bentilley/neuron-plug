/*
 * Neuron.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "../Source/MidiGenerator/WellNeurons/Neuron.hpp"
#include <catch2/catch.hpp>

SCENARIO("A Neuron")
{
  GIVEN("we have an instance of Neuron")
  {
    Neuron Neuron;

    THEN("the default values are correct")
    {
      REQUIRE(Neuron.get_threshold() == 0);
      REQUIRE(Neuron.get_state() == 0);
    }

    WHEN("we set the threshold")
    {
      Neuron.set_threshold(3);
      REQUIRE(Neuron.get_threshold() == 3);
    }

    WHEN("we get the output of the neuron with state < threshold")
    {
      Neuron.set_threshold(5);
      REQUIRE(Neuron.get_threshold() == 5);
      REQUIRE(Neuron.get_state() == 0);
      THEN("the output is 0") { REQUIRE(Neuron.get_output() == 0); }
    }

    WHEN("we get the output of the neuron with state = threshold")
    {
      REQUIRE(Neuron.get_threshold() == 0);
      REQUIRE(Neuron.get_state() == 0);
      THEN("the output of the neuron is 0") { REQUIRE(Neuron.get_output() == 0); }
    }

    WHEN("we get the output of the neuron with state > threshold")
    {
      REQUIRE(Neuron.get_threshold() == 0);
      Neuron.update_state(1);
      REQUIRE(Neuron.get_state() == 1);
      THEN("the output of the neuron is 1") { REQUIRE(Neuron.get_output() == 1); }
    }
  }

  GIVEN("we have an instance of Neuron")
  {
    Neuron Neuron;
    REQUIRE(Neuron.get_threshold() == 0); // default threshold
    REQUIRE(Neuron.get_state() == 0);     // default state

    WHEN("we perform one step of the algorithm")
    {
      Neuron.update_state(1);

      REQUIRE(Neuron.get_state() == 1);
      REQUIRE(Neuron.get_output() == 1);
    }

    WHEN("we perform two steps of the algorithm")
    {
      Neuron.update_state(1);
      Neuron.update_state(10);

      REQUIRE(Neuron.get_state() == 11);
      REQUIRE(Neuron.get_output() == 1);
    }

    WHEN("we perform three steps of the algorithm")
    {
      Neuron.update_state(1);
      Neuron.update_state(10);
      Neuron.update_state(-21);

      REQUIRE(Neuron.get_state() == -10);
      REQUIRE(Neuron.get_output() == 0);
    }
  }
}
