/*
 * Network.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "../Source/MidiGenerator/WellNeurons/Network.hpp"
#include <catch2/catch.hpp>
#include <iostream>

SCENARIO("The Network") {
  GIVEN("we have an instance of Network") {
    Network network(4);

    WHEN("we have just initialised the network the defaults are correct") {
      REQUIRE(network.num_neurons() == 4);
      REQUIRE(network.get_connection_weights().size() == 4);
      REQUIRE(network.get_connection_weights().at(0).size() == 4);
      REQUIRE(network.get_connection_weights() ==
              std::vector<std::vector<int>>{
                  std::vector<int>{0, 0, 0, 0}, std::vector<int>{0, 0, 0, 0},
                  std::vector<int>{0, 0, 0, 0}, std::vector<int>{0, 0, 0, 0}});
    }

    WHEN("we add a neuron to the network") {
      network.add_neuron();
      THEN("the neurons and connections resize") {
        REQUIRE(network.num_neurons() == 5);
        REQUIRE(network.get_connection_weights().size() == 5);
        REQUIRE(network.get_connection_weights().at(0).size() == 5);
      }
    }

    WHEN("we remove a neuron from the network") {
      network.remove_neuron();
      THEN("the neurons and connections resize") {
        REQUIRE(network.num_neurons() == 3);
        REQUIRE(network.get_connection_weights().size() == 3);
        REQUIRE(network.get_connection_weights().at(0).size() == 3);
      }
    }

    WHEN("we remove a specific  neuron from the network") {
      network.set_input_weights(std::vector<int>{1, 2, 3, 4});
      network.set_connection_weights(std::vector<std::vector<int>>{
          std::vector<int>{1, 2, 3, 4}, std::vector<int>{5, 6, 7, 8},
          std::vector<int>{9, 10, 11, 12}, std::vector<int>{13, 14, 15, 16}});
      network.remove_neuron_at(1);
      THEN("the correct conection and input weights are removed") {
        REQUIRE(network.get_input_weights() == std::vector<int>{1, 3, 4});
        REQUIRE(network.get_connection_weights() ==
                std::vector<std::vector<int>>{std::vector<int>{1, 3, 4},
                                              std::vector<int>{9, 11, 12},
                                              std::vector<int>{13, 15, 16}});
      }
    }

    WHEN("we try and set weights with the wrong size vectors it fails") {
      REQUIRE_THROWS(
          network.set_input_weights(std::vector<int>{1, 2, 3, 4, 5}));
      REQUIRE_THROWS(
          network.set_connection_weights(std::vector<std::vector<int>>{
              std::vector<int>{1, 2, 3}, std::vector<int>{4, 5, 6},
              std::vector<int>{7, 8, 9}, std::vector<int>{10, 11, 12}}));
      REQUIRE_THROWS(network.set_connection_weights(
          std::vector<std::vector<int>>{std::vector<int>{1, 3},
                                        std::vector<int>{4, 6},
                                        std::vector<int>{7, 9}}));
    }

    WHEN("we set the input weights of individual neurons") {
      network.remove_neuron();
      network.set_input_weight_for_neuron(0, 5);
      network.set_input_weight_for_neuron(1, -1);
      network.set_input_weight_for_neuron(2, 20);
      std::vector<int> input_weights = network.get_input_weights();
      REQUIRE(input_weights.at(0) == 5);
      REQUIRE(input_weights.at(1) == -1);
      REQUIRE(input_weights.at(2) == 20);
    }

    WHEN("we set the connection weights between individual neurons") {
      network.remove_neuron();
      network.set_connection_weight_for_neurons(0, 0, -17);
      network.set_connection_weight_for_neurons(0, 1, 7);
      network.set_connection_weight_for_neurons(0, 2, -1);
      network.set_connection_weight_for_neurons(1, 2, -4);
      network.set_connection_weight_for_neurons(2, 0, 9);
      std::vector<std::vector<int>> connection_weights =
          network.get_connection_weights();
      REQUIRE(connection_weights.at(0).at(0) == -17);
      REQUIRE(connection_weights.at(0).at(1) == 7);
      REQUIRE(connection_weights.at(0).at(2) == -1);
      REQUIRE(connection_weights.at(1).at(2) == -4);
      REQUIRE(connection_weights.at(2).at(0) == 9);
    }

    WHEN("we set the thresholds of individual neurons") {
      network.remove_neuron();
      network.set_threshold_for_neuron(0, 1);
      network.set_threshold_for_neuron(1, 41);
      network.set_threshold_for_neuron(2, -24);
      std::vector<Neuron> neurons = network.get_neurons();
      REQUIRE(neurons.at(0).get_threshold() == 1);
      REQUIRE(neurons.at(1).get_threshold() == 41);
      REQUIRE(neurons.at(2).get_threshold() == -24);
    }

    THEN("there is no output from the neurons in this default state") {
      REQUIRE(network.get_output() == std::vector<int>{0, 0, 0, 0});
    }
  }

  GIVEN("A network with input weights set up") {
    Network network(3);
    network.set_input_weights(std::vector<int>{1, 0, -2});

    WHEN("we have an input signal") {
      std::vector<int> input{1, 1, 1};
      THEN("the input has been multiplied by the input weights") {
        REQUIRE(network.get_weighted_input(input) ==
                std::vector<int>{1, 0, -2});
      }
    }

    WHEN("only some neurons have an input signal") {
      std::vector<int> input{0, 0, 2};
      THEN("only those neurons receive a weighted input") {
        REQUIRE(network.get_weighted_input(input) ==
                std::vector<int>{0, 0, -4});
      }
    }
  }

  GIVEN("a Network with connection weights set up") {
    Network network(3);
    network.set_connection_weights(std::vector<std::vector<int>>{
        std::vector<int>{1, 2, 3}, std::vector<int>{1, 2, 3},
        std::vector<int>{3, 2, 1}});

    WHEN("we have an output signal of (1, 1, 1)") {
      std::vector<int> output{1, 1, 1};
      THEN("the output signal gets multiplied by the connection weights") {
        REQUIRE(network.get_connection_energy(output) ==
                std::vector<int>{5, 6, 7});
      }
    }

    WHEN("we have an output signal of (1, 0, 1)") {
      std::vector<int> output{1, 0, 1};
      THEN("the output signal gets multiplied by the connection weights") {
        REQUIRE(network.get_connection_energy(output) ==
                std::vector<int>{4, 4, 4});
      }
    }

    WHEN("we have an output signal of (1, 0, 0)") {
      std::vector<int> output{1, 0, 0};
      THEN("the output signal gets multiplied by the connection weights") {
        REQUIRE(network.get_connection_energy(output) ==
                std::vector<int>{1, 2, 3});
      }
    }
  }

  GIVEN("a Network with input weights and connection weights set up") {
    Network network(3);
    network.set_input_weights(std::vector<int>{1, 0, -2});
    network.set_connection_weights(std::vector<std::vector<int>>{
        std::vector<int>{1, 2, 3}, std::vector<int>{1, 2, 3},
        std::vector<int>{1, 2, 3}});

    WHEN("we advance the state") {
      network.process_next(std::vector<int>{3, -1, 8});

      THEN("the neurons update their state and clear their input") {
        std::vector<Neuron> n = network.get_neurons();
        std::vector<int> states(network.num_neurons(), 0);
        std::vector<int> inputs(network.num_neurons(), 0);
        std::transform(n.begin(), n.end(), states.begin(),
                       [](Neuron n) { return n.get_state(); });
        std::transform(n.begin(), n.end(), inputs.begin(),
                       [](Neuron n) { return n.get_input(); });
        REQUIRE(states == std::vector<int>{3, 0, -16});
        REQUIRE(inputs == std::vector<int>{0, 0, 0});
      }
    }
  }

  GIVEN("a Network with 2 neurons with input and connection weights setup") {
    Network network(2);
    network.set_input_weights(std::vector<int>{1, 1});
    network.set_connection_weights(std::vector<std::vector<int>>{
        std::vector<int>{-3, 1}, std::vector<int>{1, -3}});

    WHEN("we advance the state once") {
      network.process_next(std::vector<int>{1, 1});
      REQUIRE(network.get_output() == std::vector<int>{1, 1});
    }

    WHEN("we advance the state twice") {
      network.process_next(std::vector<int>{1, 1});
      network.process_next(std::vector<int>{1, 1});
      REQUIRE(network.get_output() == std::vector<int>{0, 0});
    }

    WHEN("we advance the state thrice") {
      network.process_next(std::vector<int>{1, 1});
      network.process_next(std::vector<int>{1, 1});
      network.process_next(std::vector<int>{1, 0});
      REQUIRE(network.get_output() == std::vector<int>{1, 0});
    }
  }

  GIVEN("a Network with 2 neurons with input and connection weights setup") {
    Network network(2);
    network.set_input_weights(std::vector<int>{1, 1});
    network.set_connection_weights(std::vector<std::vector<int>>{
        std::vector<int>{-3, 1}, std::vector<int>{1, -2}});

    THEN("it should handle multiple state updates") {
      network.process_next(std::vector<int>{1, 1});
      REQUIRE(network.get_output() == std::vector<int>{1, 1});
      network.process_next(std::vector<int>{0, 0});
      REQUIRE(network.get_output() == std::vector<int>{0, 0});
      network.process_next(std::vector<int>{1, 1});
      REQUIRE(network.get_output() == std::vector<int>{0, 1});
      network.process_next(std::vector<int>{0, 0});
      REQUIRE(network.get_output() == std::vector<int>{1, 0});
      network.process_next(std::vector<int>{1, 0});
      REQUIRE(network.get_output() == std::vector<int>{0, 0});
      network.process_next(std::vector<int>{0, 1});
      REQUIRE(network.get_output() == std::vector<int>{0, 1});
      network.process_next(std::vector<int>{1, 0});
      REQUIRE(network.get_output() == std::vector<int>{1, 0});
      network.process_next(std::vector<int>{0, 1});
      REQUIRE(network.get_output() == std::vector<int>{0, 1});
    }
  }

  GIVEN("a Network with 4 neurons with input and connection weights setup") {
    Network network(3);
    network.set_input_weights(std::vector<int>{1, 1, 1});
    network.set_connection_weights(std::vector<std::vector<int>>{
        std::vector<int>{-3, 1, 4}, std::vector<int>{1, -2, 1},
        std::vector<int>{2, 1, -7}});
    network.add_neuron();
    network.set_input_weight_for_neuron(3, 2);
    network.set_connection_weight_for_neurons(0, 3, 1);
    network.set_connection_weight_for_neurons(1, 3, 1);
    network.set_connection_weight_for_neurons(2, 3, -2);
    network.set_connection_weight_for_neurons(3, 0, 2);
    network.set_connection_weight_for_neurons(3, 1, -2);
    network.set_connection_weight_for_neurons(3, 3, -3);
    network.set_threshold_for_neuron(0, 1);

    THEN("it should handle multiple state updates") {
      network.process_next(std::vector<int>{1, 1, 1, 0});
      REQUIRE(network.get_output() == std::vector<int>{0, 1, 1, 0});
      network.process_next(std::vector<int>{0, 0, 0, 1});
      REQUIRE(network.get_output() == std::vector<int>{1, 0, 0, 1});
      network.process_next(std::vector<int>{1, 1, 1, 0});
      REQUIRE(network.get_output() == std::vector<int>{1, 0, 0, 0});
      network.process_next(std::vector<int>{0, 0, 0, 1});
      REQUIRE(network.get_output() == std::vector<int>{0, 1, 1, 1});
      network.process_next(std::vector<int>{1, 0, 1, 1});
      REQUIRE(network.get_output() == std::vector<int>{1, 0, 0, 0});
      network.process_next(std::vector<int>{0, 1, 0, 0});
      REQUIRE(network.get_output() == std::vector<int>{1, 0, 1, 1});
      network.process_next(std::vector<int>{1, 0, 1, 1});
      REQUIRE(network.get_output() == std::vector<int>{1, 0, 1, 0});
      network.process_next(std::vector<int>{0, 1, 0, 0});
      REQUIRE(network.get_output() == std::vector<int>{1, 1, 0, 0});
    }
  }
}
