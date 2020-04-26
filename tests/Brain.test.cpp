/*
 * Brain.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "../Source/MidiGenerator/WellNeurons/Brain.hpp"
#include <catch2/catch.hpp>
#include <iostream>

SCENARIO("The Brain") {
  GIVEN("we have an instance of Brain") {
    Brain brain;

    WHEN("we have just initialised the brain the defaults are correct") {
      REQUIRE(brain.num_neurons() == 3);
      REQUIRE(brain.get_connection_weights().size() == 3);
      REQUIRE(brain.get_connection_weights().at(0).size() == 3);
      REQUIRE(brain.get_connection_weights() ==
              std::vector<std::vector<int>>{std::vector<int>{0, 0, 0},
                                            std::vector<int>{0, 0, 0},
                                            std::vector<int>{0, 0, 0}});
    }

    WHEN("we add a neuron to the brain") {
      brain.add_neuron();
      THEN("the neurons and connections resize") {
        REQUIRE(brain.num_neurons() == 4);
        REQUIRE(brain.get_connection_weights().size() == 4);
        REQUIRE(brain.get_connection_weights().at(0).size() == 4);
      }
    }

    WHEN("we remove a neuron from the brain") {
      brain.remove_neuron();
      THEN("the neurons and connections resize") {
        REQUIRE(brain.num_neurons() == 2);
        REQUIRE(brain.get_connection_weights().size() == 2);
        REQUIRE(brain.get_connection_weights().at(0).size() == 2);
      }
    }

    WHEN("we remove a specific  neuron from the brain") {
      brain.set_input_weights(std::vector<int>{1, 2, 3});
      brain.set_connection_weights(std::vector<std::vector<int>>{
          std::vector<int>{1, 2, 3}, std::vector<int>{4, 5, 6},
          std::vector<int>{7, 8, 9}});
      brain.remove_neuron(2);
      THEN("the correct conection and input weights are removed") {
        REQUIRE(brain.get_input_weights() == std::vector<int>{1, 3});
        REQUIRE(brain.get_connection_weights() ==
                std::vector<std::vector<int>>{std::vector<int>{1, 3},
                                              std::vector<int>{7, 9}});
      }
    }

    WHEN("we try and set weights with the wrong size vectors it fails") {
      REQUIRE_THROWS(brain.set_input_weights(std::vector<int>{1, 2, 3, 4}));
      REQUIRE_THROWS(brain.set_connection_weights(std::vector<std::vector<int>>{
          std::vector<int>{1, 2, 3}, std::vector<int>{4, 5, 6},
          std::vector<int>{7, 8, 9}, std::vector<int>{10, 11, 12}}));
      REQUIRE_THROWS(brain.set_connection_weights(std::vector<std::vector<int>>{
          std::vector<int>{1, 3}, std::vector<int>{4, 6},
          std::vector<int>{7, 9}}));
    }

    WHEN("we set the input weights of individual neurons") {
      brain.set_input_weight_for_neuron(0, 5);
      brain.set_input_weight_for_neuron(1, -1);
      brain.set_input_weight_for_neuron(2, 20);
      std::vector<int> input_weights = brain.get_input_weights();
      REQUIRE(input_weights.at(0) == 5);
      REQUIRE(input_weights.at(1) == -1);
      REQUIRE(input_weights.at(2) == 20);
    }

    WHEN("we set the connection weights between individual neurons") {
      brain.set_connection_weight_for_neurons(0, 0, -17);
      brain.set_connection_weight_for_neurons(0, 1, 7);
      brain.set_connection_weight_for_neurons(0, 2, -1);
      brain.set_connection_weight_for_neurons(1, 2, -4);
      brain.set_connection_weight_for_neurons(2, 0, 9);
      std::vector<std::vector<int>> connection_weights =
          brain.get_connection_weights();
      REQUIRE(connection_weights.at(0).at(0) == -17);
      REQUIRE(connection_weights.at(0).at(1) == 7);
      REQUIRE(connection_weights.at(0).at(2) == -1);
      REQUIRE(connection_weights.at(1).at(2) == -4);
      REQUIRE(connection_weights.at(2).at(0) == 9);
    }

    WHEN("we set the thresholds of individual neurons") {
      brain.set_threshold_for_neuron(0, 1);
      brain.set_threshold_for_neuron(1, 41);
      brain.set_threshold_for_neuron(2, -24);
      std::vector<Neuron> neurons = brain.get_neurons();
      REQUIRE(neurons.at(0).get_threshold() == 1);
      REQUIRE(neurons.at(1).get_threshold() == 41);
      REQUIRE(neurons.at(2).get_threshold() == -24);
    }

    THEN("there is no output from the neurons in this default state") {
      REQUIRE(brain.get_output() == std::vector<int>{0, 0, 0});
    }
  }

  GIVEN("A brain with input weights set up") {
    Brain brain;
    brain.set_input_weights(std::vector<int>{1, 0, -2});

    WHEN("we have an input signal") {
      std::vector<int> input{1, 1, 1};
      THEN("the input has been multiplied by the input weights") {
        REQUIRE(brain.get_weighted_input(input) == std::vector<int>{1, 0, -2});
      }
    }

    WHEN("only some neurons have an input signal") {
      std::vector<int> input{0, 0, 2};
      THEN("only those neurons receive a weighted input") {
        REQUIRE(brain.get_weighted_input(input) == std::vector<int>{0, 0, -4});
      }
    }
  }

  GIVEN("a Brain with connection weights set up") {
    Brain brain;
    brain.set_connection_weights(std::vector<std::vector<int>>{
        std::vector<int>{1, 2, 3}, std::vector<int>{1, 2, 3},
        std::vector<int>{3, 2, 1}});

    WHEN("we have an output signal of (1, 1, 1)") {
      std::vector<int> output{1, 1, 1};
      THEN("the output signal gets multiplied by the connection weights") {
        REQUIRE(brain.get_connection_energy(output) ==
                std::vector<int>{5, 6, 7});
      }
    }

    WHEN("we have an output signal of (1, 0, 1)") {
      std::vector<int> output{1, 0, 1};
      THEN("the output signal gets multiplied by the connection weights") {
        REQUIRE(brain.get_connection_energy(output) ==
                std::vector<int>{4, 4, 4});
      }
    }

    WHEN("we have an output signal of (1, 0, 0)") {
      std::vector<int> output{1, 0, 0};
      THEN("the output signal gets multiplied by the connection weights") {
        REQUIRE(brain.get_connection_energy(output) ==
                std::vector<int>{1, 2, 3});
      }
    }
  }

  GIVEN("a Brain with input weights and connection weights set up") {
    Brain brain;
    brain.set_input_weights(std::vector<int>{1, 0, -2});
    brain.set_connection_weights(std::vector<std::vector<int>>{
        std::vector<int>{1, 2, 3}, std::vector<int>{1, 2, 3},
        std::vector<int>{1, 2, 3}});

    WHEN("we advance the state") {
      brain.process_next(std::vector<int>{3, -1, 8});

      THEN("the neurons update their state and clear their input") {
        std::vector<Neuron> n = brain.get_neurons();
        std::vector<int> states(brain.num_neurons(), 0);
        std::vector<int> inputs(brain.num_neurons(), 0);
        std::transform(n.begin(), n.end(), states.begin(),
                       [](Neuron n) { return n.get_state(); });
        std::transform(n.begin(), n.end(), inputs.begin(),
                       [](Neuron n) { return n.get_input(); });
        REQUIRE(states == std::vector<int>{3, 0, -16});
        REQUIRE(inputs == std::vector<int>{0, 0, 0});
      }
    }
  }

  GIVEN("a Brain with 2 neurons with input and connection weights setup") {
    Brain brain;
    brain.remove_neuron();
    brain.set_input_weights(std::vector<int>{1, 1});
    brain.set_connection_weights(std::vector<std::vector<int>>{
        std::vector<int>{-3, 1}, std::vector<int>{1, -3}});

    WHEN("we advance the state once") {
      brain.process_next(std::vector<int>{1, 1});
      REQUIRE(brain.get_output() == std::vector<int>{1, 1});
    }

    WHEN("we advance the state twice") {
      brain.process_next(std::vector<int>{1, 1});
      brain.process_next(std::vector<int>{1, 1});
      REQUIRE(brain.get_output() == std::vector<int>{0, 0});
    }

    WHEN("we advance the state thrice") {
      brain.process_next(std::vector<int>{1, 1});
      brain.process_next(std::vector<int>{1, 1});
      brain.process_next(std::vector<int>{1, 0});
      REQUIRE(brain.get_output() == std::vector<int>{1, 0});
    }
  }

  GIVEN("a Brain with 2 neurons with input and connection weights setup") {
    Brain brain;
    brain.remove_neuron();
    brain.set_input_weights(std::vector<int>{1, 1});
    brain.set_connection_weights(std::vector<std::vector<int>>{
        std::vector<int>{-3, 1}, std::vector<int>{1, -2}});

    THEN("it should handle multiple state updates") {
      brain.process_next(std::vector<int>{1, 1});
      REQUIRE(brain.get_output() == std::vector<int>{1, 1});
      brain.process_next(std::vector<int>{0, 0});
      REQUIRE(brain.get_output() == std::vector<int>{0, 0});
      brain.process_next(std::vector<int>{1, 1});
      REQUIRE(brain.get_output() == std::vector<int>{0, 1});
      brain.process_next(std::vector<int>{0, 0});
      REQUIRE(brain.get_output() == std::vector<int>{1, 0});
      brain.process_next(std::vector<int>{1, 0});
      REQUIRE(brain.get_output() == std::vector<int>{0, 0});
      brain.process_next(std::vector<int>{0, 1});
      REQUIRE(brain.get_output() == std::vector<int>{0, 1});
      brain.process_next(std::vector<int>{1, 0});
      REQUIRE(brain.get_output() == std::vector<int>{1, 0});
      brain.process_next(std::vector<int>{0, 1});
      REQUIRE(brain.get_output() == std::vector<int>{0, 1});
    }
  }

  GIVEN("a Brain with 4 neurons with input and connection weights setup") {
    Brain brain;
    brain.set_input_weights(std::vector<int>{1, 1, 1});
    brain.set_connection_weights(std::vector<std::vector<int>>{
        std::vector<int>{-3, 1, 4}, std::vector<int>{1, -2, 1},
        std::vector<int>{2, 1, -7}});
    brain.add_neuron();
    brain.set_input_weight_for_neuron(3, 2);
    brain.set_connection_weight_for_neurons(0, 3, 1);
    brain.set_connection_weight_for_neurons(1, 3, 1);
    brain.set_connection_weight_for_neurons(2, 3, -2);
    brain.set_connection_weight_for_neurons(3, 0, 2);
    brain.set_connection_weight_for_neurons(3, 1, -2);
    brain.set_connection_weight_for_neurons(3, 3, -3);
    brain.set_threshold_for_neuron(0, 1);

    THEN("it should handle multiple state updates") {
      brain.process_next(std::vector<int>{1, 1, 1, 0});
      REQUIRE(brain.get_output() == std::vector<int>{0, 1, 1, 0});
      brain.process_next(std::vector<int>{0, 0, 0, 1});
      REQUIRE(brain.get_output() == std::vector<int>{1, 0, 0, 1});
      brain.process_next(std::vector<int>{1, 1, 1, 0});
      REQUIRE(brain.get_output() == std::vector<int>{1, 0, 0, 0});
      brain.process_next(std::vector<int>{0, 0, 0, 1});
      REQUIRE(brain.get_output() == std::vector<int>{0, 1, 1, 1});
      brain.process_next(std::vector<int>{1, 0, 1, 1});
      REQUIRE(brain.get_output() == std::vector<int>{1, 0, 0, 0});
      brain.process_next(std::vector<int>{0, 1, 0, 0});
      REQUIRE(brain.get_output() == std::vector<int>{1, 0, 1, 1});
      brain.process_next(std::vector<int>{1, 0, 1, 1});
      REQUIRE(brain.get_output() == std::vector<int>{1, 0, 1, 0});
      brain.process_next(std::vector<int>{0, 1, 0, 0});
      REQUIRE(brain.get_output() == std::vector<int>{1, 1, 0, 0});
    }
  }
}
