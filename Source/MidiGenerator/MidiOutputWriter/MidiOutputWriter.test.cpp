/*
 * MidiOutputWriter.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Brain/io.hpp"
#include <catch2/catch.hpp>
#include <vector>

#define private public
#include "MidiOutputWriter.hpp"

using Type = ModelVector::InputType;

SCENARIO("MidiOutputWriter") {
  GIVEN("An instance of MidiOutputWriter with no notes playing") {
    MidiOutputWriter writer;
    MidiBuffer buffer;

    GIVEN("Model output with only one ModelVector with one activation") {
      ModelVectorData data;
      data.at(60) = 1.0f; // activation at note 60
      int sampleNumber = 87;
      ModelVector vec1{data, sampleNumber, Type::MergedInput};
      std::vector<ModelVector> output{vec1};

      writer.writeMidiOutput(output, buffer);

      REQUIRE(buffer.getNumEvents() == 1);
      int time;
      MidiMessage m;
      MidiBuffer::Iterator i(buffer);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(m.getFloatVelocity() == 1.0f);
      REQUIRE(time == 87);
    }

    GIVEN("Model output with one ModelVector with multiple activations") {
      ModelVectorData data;
      data.at(60) = 1.0f; // activation at note 60
      data.at(68) = 0.8f; // activation at note 68
      data.at(89) = 0.3f; // activation at note 89
      int sampleNumber = 145;
      ModelVector vec1{data, sampleNumber, Type::MergedInput};
      std::vector<ModelVector> output{vec1};

      writer.writeMidiOutput(output, buffer);

      REQUIRE(buffer.getNumEvents() == 3);
      int time;
      MidiMessage m;
      MidiBuffer::Iterator i(buffer);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(m.getFloatVelocity() == 1.0f);
      REQUIRE(time == 145);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 68);
      REQUIRE(m.getFloatVelocity() == 0.8f);
      REQUIRE(time == 145);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 89);
      REQUIRE(m.getFloatVelocity() == 0.3f);
      REQUIRE(time == 145);
    }

    GIVEN("Model output with multiple ModelVector's with one activation each") {
      ModelVectorData data1;
      data1.at(60) = 1.0f; // activation at note 60
      int sampleNumber1 = 145;
      ModelVector vec1{data1, sampleNumber1, Type::MergedInput};

      ModelVectorData data2;
      data2.at(33) = 0.27f; // activation at note 33
      int sampleNumber2 = 172;
      ModelVector vec2{data1, sampleNumber2, Type::MergedInput};

      ModelVectorData data3;
      data3.at(103) = 0.345f; // activation at note 103
      int sampleNumber3 = 239;
      ModelVector vec3{data1, sampleNumber3, Type::MergedInput};

      std::vector<ModelVector> output{vec1, vec2, vec3};

      writer.writeMidiOutput(output, buffer);

      REQUIRE(buffer.getNumEvents() == 3);
      int time;
      MidiMessage m;
      MidiBuffer::Iterator i(buffer);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(m.getFloatVelocity() == 1.0f);
      REQUIRE(time == 145);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 33);
      REQUIRE(m.getFloatVelocity() == 0.27f);
      REQUIRE(time == 172);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 103);
      REQUIRE(m.getFloatVelocity() == 0.345f);
      REQUIRE(time == 239);
    }

    GIVEN(
        "Model output with multiple ModelVector's with multiple activations") {
      ModelVectorData data1;
      data1.at(60) = 1.0f; // activation at note 60
      data1.at(18) = 0.9f; // activation at note 18
      int sampleNumber1 = 31;
      ModelVector vec1{data1, sampleNumber1, Type::MergedInput};

      ModelVectorData data2;
      data2.at(33) = 0.27f; // activation at note 33
      data2.at(51) = 0.82f; // activation at note 51
      int sampleNumber2 = 48;
      ModelVector vec2{data1, sampleNumber2, Type::MergedInput};

      ModelVectorData data3;
      data3.at(103) = 0.97f; // activation at note 103
      data3.at(76) = 0.175f; // activation at note 76
      int sampleNumber3 = 199;
      ModelVector vec3{data1, sampleNumber3, Type::MergedInput};

      std::vector<ModelVector> output{vec1, vec2, vec3};

      writer.writeMidiOutput(output, buffer);

      REQUIRE(buffer.getNumEvents() == 6);
      int time;
      MidiMessage m;
      MidiBuffer::Iterator i(buffer);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(m.getFloatVelocity() == 1.0f);
      REQUIRE(time == 31);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 18);
      REQUIRE(m.getFloatVelocity() == 0.9f);
      REQUIRE(time == 31);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 33);
      REQUIRE(m.getFloatVelocity() == 0.27f);
      REQUIRE(time == 48);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 51);
      REQUIRE(m.getFloatVelocity() == 0.82f);
      REQUIRE(time == 48);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 103);
      REQUIRE(m.getFloatVelocity() == 0.97f);
      REQUIRE(time == 199);
      i.getNextEvent(m, time);
      REQUIRE(m.getNoteNumber() == 76);
      REQUIRE(m.getFloatVelocity() == 0.175f);
      REQUIRE(time == 199);
    }
  }
}

SCENARIO("sampleNumModuloBufferSize") {
  GIVEN("a vector of ModelVector's") {
    ModelVector vector1{0.3f, 345905, Type::MergedInput};
    ModelVector vector2{0.4f, 346048, Type::MergedInput};
    ModelVector vector3{0.5f, 346067, Type::MergedInput};
    std::vector<ModelVector> modelOutput{vector1, vector2, vector3};

    THEN("with buffer size of 256") {
      int bufferSize = 256;

      auto result = sampleNumModuloBufferSize(modelOutput, bufferSize);

      REQUIRE(result.at(0).sampleNumber == 49);
      REQUIRE(result.at(1).sampleNumber == 192);
      REQUIRE(result.at(2).sampleNumber == 211);
    }

    THEN("with buffer size of 512") {
      int bufferSize = 512;

      auto result = sampleNumModuloBufferSize(modelOutput, bufferSize);

      REQUIRE(result.at(0).sampleNumber == 305);
      REQUIRE(result.at(1).sampleNumber == 448);
      REQUIRE(result.at(2).sampleNumber == 467);
    }
  }
}
