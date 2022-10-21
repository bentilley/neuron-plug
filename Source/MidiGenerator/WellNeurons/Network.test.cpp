/*
 * Network.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "../Source/MidiGenerator/WellNeurons/Network.hpp"
#include <catch2/catch.hpp>
#include <iostream>

using Points = std::vector<std::pair<int, float>>;

ModelVector getModelVector(Points points)
{
  return ModelVector(points, 0, ModelVector::InputType::MidiInput);
}

SCENARIO("The Network")
{
  GIVEN("we have an instance of Network")
  {
    Network network(4);

    WHEN("we have just initialised the network the defaults are correct")
    {
      REQUIRE(network.numNeurons() == 4);
      REQUIRE(network.getConnectionWeights().size() == 4);
      REQUIRE(network.getConnectionWeights().at(0).size() == 4);
      REQUIRE(
        network.getConnectionWeights() ==
        std::vector<std::vector<int>>{
          std::vector<int>{0, 0, 0, 0},
          std::vector<int>{0, 0, 0, 0},
          std::vector<int>{0, 0, 0, 0},
          std::vector<int>{0, 0, 0, 0}}
      );
    }

    WHEN("we add a neuron to the network")
    {
      network.addNeuron();
      THEN("the neurons and connections resize")
      {
        REQUIRE(network.numNeurons() == 5);
        REQUIRE(network.getConnectionWeights().size() == 5);
        REQUIRE(network.getConnectionWeights().at(0).size() == 5);
      }
    }

    WHEN("we remove a neuron from the network")
    {
      network.removeNeuron();
      THEN("the neurons and connections resize")
      {
        REQUIRE(network.numNeurons() == 3);
        REQUIRE(network.getConnectionWeights().size() == 3);
        REQUIRE(network.getConnectionWeights().at(0).size() == 3);
      }
    }

    WHEN("we remove a specific  neuron from the network")
    {
      network.setInputWeights(std::vector<int>{1, 2, 3, 4});
      network.setConnectionWeights(std::vector<std::vector<int>>{
        std::vector<int>{1, 2, 3, 4},
        std::vector<int>{5, 6, 7, 8},
        std::vector<int>{9, 10, 11, 12},
        std::vector<int>{13, 14, 15, 16}});
      network.removeNeuronAt(1);
      THEN("the correct conection and input weights are removed")
      {
        REQUIRE(network.getInputWeights() == std::vector<int>{1, 3, 4});
        REQUIRE(
          network.getConnectionWeights() ==
          std::vector<std::vector<int>>{
            std::vector<int>{1, 3, 4},
            std::vector<int>{9, 11, 12},
            std::vector<int>{13, 15, 16}}
        );
      }
    }

    WHEN("we try and set weights with the wrong size vectors it fails")
    {
      REQUIRE_THROWS(network.setInputWeights(std::vector<int>{1, 2, 3, 4, 5}));
      REQUIRE_THROWS(network.setConnectionWeights(std::vector<std::vector<int>>{
        std::vector<int>{1, 2, 3},
        std::vector<int>{4, 5, 6},
        std::vector<int>{7, 8, 9},
        std::vector<int>{10, 11, 12}}));
      REQUIRE_THROWS(network.setConnectionWeights(std::vector<std::vector<int>>{
        std::vector<int>{1, 3},
        std::vector<int>{4, 6},
        std::vector<int>{7, 9}}));
    }

    WHEN("we set the input weights of individual neurons")
    {
      network.removeNeuron();
      network.setInputWeightForNeuron(0, 5);
      network.setInputWeightForNeuron(1, -1);
      network.setInputWeightForNeuron(2, 20);
      std::vector<int> input_weights = network.getInputWeights();
      REQUIRE(input_weights.at(0) == 5);
      REQUIRE(input_weights.at(1) == -1);
      REQUIRE(input_weights.at(2) == 20);
    }

    WHEN("we set the connection weights between individual neurons")
    {
      network.removeNeuron();
      network.setConnectionWeightForNeurons(0, 0, -17);
      network.setConnectionWeightForNeurons(0, 1, 7);
      network.setConnectionWeightForNeurons(0, 2, -1);
      network.setConnectionWeightForNeurons(1, 2, -4);
      network.setConnectionWeightForNeurons(2, 0, 9);
      std::vector<std::vector<int>> connection_weights = network.getConnectionWeights();
      REQUIRE(connection_weights.at(0).at(0) == -17);
      REQUIRE(connection_weights.at(0).at(1) == 7);
      REQUIRE(connection_weights.at(0).at(2) == -1);
      REQUIRE(connection_weights.at(1).at(2) == -4);
      REQUIRE(connection_weights.at(2).at(0) == 9);
    }

    WHEN("we set the thresholds of individual neurons")
    {
      network.removeNeuron();
      network.setThresholdForNeuron(0, 1);
      network.setThresholdForNeuron(1, 41);
      network.setThresholdForNeuron(2, -24);
      std::vector<Neuron> neurons = network.getNeurons();
      REQUIRE(neurons.at(0).get_threshold() == 1);
      REQUIRE(neurons.at(1).get_threshold() == 41);
      REQUIRE(neurons.at(2).get_threshold() == -24);
    }

    THEN("there is no output from the neurons in this default state")
    {
      REQUIRE(network.getOutput() == std::vector<float>{0, 0, 0, 0});
    }
  }

  GIVEN("A network with input weights set up")
  {
    Network network(3);
    network.setInputWeights(std::vector<int>{1, 0, -2});

    WHEN("we have an input signal")
    {
      std::vector<float> input{1, 1, 1};
      THEN("the input has been multiplied by the input weights")
      {
        REQUIRE(network.getWeightedInput(input) == std::vector<float>{1, 0, -2});
      }
    }

    WHEN("only some neurons have an input signal")
    {
      std::vector<float> input{0, 0, 2};
      THEN("only those neurons receive a weighted input")
      {
        REQUIRE(network.getWeightedInput(input) == std::vector<float>{0, 0, -4});
      }
    }
  }

  GIVEN("a Network with connection weights set up")
  {
    Network network(3);
    network.setConnectionWeights(std::vector<std::vector<int>>{
      std::vector<int>{1, 2, 3},
      std::vector<int>{1, 2, 3},
      std::vector<int>{3, 2, 1}});

    WHEN("we have an output signal of (1, 1, 1)")
    {
      std::vector<float> output{1, 1, 1};
      THEN("the output signal gets multiplied by the connection weights")
      {
        REQUIRE(network.getConnectionEnergy(output) == std::vector<float>{5, 6, 7});
      }
    }

    WHEN("we have an output signal of (1, 0, 1)")
    {
      std::vector<float> output{1, 0, 1};
      THEN("the output signal gets multiplied by the connection weights")
      {
        REQUIRE(network.getConnectionEnergy(output) == std::vector<float>{4, 4, 4});
      }
    }

    WHEN("we have an output signal of (1, 0, 0)")
    {
      std::vector<float> output{1, 0, 0};
      THEN("the output signal gets multiplied by the connection weights")
      {
        REQUIRE(network.getConnectionEnergy(output) == std::vector<float>{1, 2, 3});
      }
    }
  }

  GIVEN("a Network with input weights and connection weights set up")
  {
    Network network(3);
    network.setInputWeights(std::vector<int>{1, 0, -2});
    network.setConnectionWeights(std::vector<std::vector<int>>{
      std::vector<int>{1, 2, 3},
      std::vector<int>{1, 2, 3},
      std::vector<int>{1, 2, 3}});
    network.setNeuronMidiNotes(std::vector<int>{60, 64, 67});

    WHEN("we advance the state")
    {
      network.processNext(getModelVector(Points{{60, 3}, {64, -1}, {67, 8}}));

      THEN("the neurons update their state and clear their input")
      {
        std::vector<Neuron> n = network.getNeurons();
        std::vector<int> states(network.numNeurons(), 0);
        std::transform(n.begin(), n.end(), states.begin(), [](Neuron n) { return n.get_state(); });
        REQUIRE(states == std::vector<int>{3, 0, -16});
      }
    }
  }

  GIVEN("a Network with 2 neurons with input and connection weights setup")
  {
    Network network(2);
    network.setInputWeights(std::vector<int>{1, 1});
    network.setConnectionWeights(
      std::vector<std::vector<int>>{std::vector<int>{-3, 1}, std::vector<int>{1, -3}}
    );
    network.setNeuronMidiNotes(std::vector<int>{60, 64});

    WHEN("we advance the state once")
    {
      network.processNext(getModelVector(Points{{60, 1}, {64, 1}}));
      REQUIRE(network.getOutput() == std::vector<float>{1, 1});
    }

    WHEN("we advance the state twice")
    {
      network.processNext(getModelVector(Points{{60, 1}, {64, 1}}));
      network.processNext(getModelVector(Points{{60, 1}, {64, 1}}));
      REQUIRE(network.getOutput() == std::vector<float>{0, 0});
    }

    WHEN("we advance the state thrice")
    {
      network.processNext(getModelVector(Points{{60, 1}, {64, 1}}));
      network.processNext(getModelVector(Points{{60, 1}, {64, 1}}));
      network.processNext(getModelVector(Points{{60, 1}, {64, 0}}));
      REQUIRE(network.getOutput() == std::vector<float>{1, 0});
    }
  }

  GIVEN("a Network with 2 neurons with input and connection weights setup")
  {
    Network network(2);
    network.setInputWeights(std::vector<int>{1, 1});
    network.setConnectionWeights(
      std::vector<std::vector<int>>{std::vector<int>{-3, 1}, std::vector<int>{1, -2}}
    );
    network.setNeuronMidiNotes(std::vector<int>{60, 64});

    THEN("it should handle multiple state updates")
    {
      network.processNext(getModelVector(Points{{60, 1}, {64, 1}}));
      REQUIRE(network.getOutput() == std::vector<float>{1, 1});
      network.processNext(getModelVector(Points{{60, 0}, {64, 0}}));
      REQUIRE(network.getOutput() == std::vector<float>{0, 0});
      network.processNext(getModelVector(Points{{60, 1}, {64, 1}}));
      REQUIRE(network.getOutput() == std::vector<float>{0, 1});
      network.processNext(getModelVector(Points{{60, 0}, {64, 0}}));
      REQUIRE(network.getOutput() == std::vector<float>{1, 0});
      network.processNext(getModelVector(Points{{60, 1}, {64, 0}}));
      REQUIRE(network.getOutput() == std::vector<float>{0, 0});
      network.processNext(getModelVector(Points{{60, 0}, {64, 1}}));
      REQUIRE(network.getOutput() == std::vector<float>{0, 1});
      network.processNext(getModelVector(Points{{60, 1}, {64, 0}}));
      REQUIRE(network.getOutput() == std::vector<float>{1, 0});
      network.processNext(getModelVector(Points{{60, 0}, {64, 1}}));
      REQUIRE(network.getOutput() == std::vector<float>{0, 1});
    }
  }

  GIVEN("a Network with 4 neurons with input and connection weights setup")
  {
    Network network(3);
    network.setInputWeights(std::vector<int>{1, 1, 1});
    network.setConnectionWeights(std::vector<std::vector<int>>{
      std::vector<int>{-3, 1, 4},
      std::vector<int>{1, -2, 1},
      std::vector<int>{2, 1, -7}});
    network.addNeuron();
    network.setInputWeightForNeuron(3, 2);
    network.setConnectionWeightForNeurons(0, 3, 1);
    network.setConnectionWeightForNeurons(1, 3, 1);
    network.setConnectionWeightForNeurons(2, 3, -2);
    network.setConnectionWeightForNeurons(3, 0, 2);
    network.setConnectionWeightForNeurons(3, 1, -2);
    network.setConnectionWeightForNeurons(3, 3, -3);
    network.setThresholdForNeuron(0, 1);
    network.setNeuronMidiNotes(std::vector<int>{60, 64, 67, 71});

    THEN("it should handle multiple state updates")
    {
      network.processNext(getModelVector(Points{{60, 1}, {64, 1}, {67, 1}, {71, 0}}));
      REQUIRE(network.getOutput() == std::vector<float>{0, 1, 1, 0});
      network.processNext(getModelVector(Points{{60, 0}, {64, 0}, {67, 0}, {71, 1}}));
      REQUIRE(network.getOutput() == std::vector<float>{1, 0, 0, 1});
      network.processNext(getModelVector(Points{{60, 1}, {64, 1}, {67, 1}, {71, 0}}));
      REQUIRE(network.getOutput() == std::vector<float>{1, 0, 0, 0});
      network.processNext(getModelVector(Points{{60, 0}, {64, 0}, {67, 0}, {71, 1}}));
      REQUIRE(network.getOutput() == std::vector<float>{0, 1, 1, 1});
      network.processNext(getModelVector(Points{{60, 1}, {64, 0}, {67, 1}, {71, 1}}));
      REQUIRE(network.getOutput() == std::vector<float>{1, 0, 0, 0});
      network.processNext(getModelVector(Points{{60, 0}, {64, 1}, {67, 0}, {71, 0}}));
      REQUIRE(network.getOutput() == std::vector<float>{1, 0, 1, 1});
      network.processNext(getModelVector(Points{{60, 1}, {64, 0}, {67, 1}, {71, 1}}));
      REQUIRE(network.getOutput() == std::vector<float>{1, 0, 1, 0});
      network.processNext(getModelVector(Points{{60, 0}, {64, 1}, {67, 0}, {71, 0}}));
      REQUIRE(network.getOutput() == std::vector<float>{1, 1, 0, 0});
    }
  }
}
