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
  GIVEN("an instance of MidiOutputWriter using the maxNoteLength constructor") {
    MidiOutputWriter writer{59934};

    REQUIRE(writer.limitNoteLength);
    REQUIRE(writer.maxNoteLength == 59934);
  }

  GIVEN("An instance of MidiOutputWriter with no notes playing") {
    MidiOutputWriter writer;
    MidiBuffer buffer;
    int bufferSize = 256;

    THEN("we test the defaults") {
      REQUIRE(!writer.limitNoteLength);
      REQUIRE(writer.maxNoteLength == 0);

      THEN("we set the maxNoteLength") {
        writer.setMaxNoteLength(1024);

        REQUIRE(writer.limitNoteLength);
        REQUIRE(writer.maxNoteLength == 1024);

        THEN("we disable the max note length") {
          writer.disableMaxNoteLength();

          REQUIRE(!writer.limitNoteLength);
          REQUIRE(writer.maxNoteLength == 0);
        }
      }
    }

    GIVEN("Model output with only one ModelVector with one activation") {
      ModelVectorData data;
      data.at(60) = 1.0f; // activation at note 60
      int sampleNumber = 87;
      ModelVector vec1{data, sampleNumber, Type::MergedInput};
      std::vector<ModelVector> output{vec1};

      writer.writeMidiOutput(output, buffer, bufferSize);

      REQUIRE(buffer.getNumEvents() == 1);
      int time;
      MidiMessage m;
      MidiBuffer::Iterator i(buffer);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(m.getFloatVelocity() == 1.0f);
      REQUIRE(time == 87);

      // check the samples played so far is correct
      REQUIRE(writer.playingMidiNotes.at(60) == 168);
    }

    GIVEN("Model output with one ModelVector with multiple activations") {
      ModelVectorData data;
      data.at(60) = 1.0f; // activation at note 60
      data.at(68) = 0.8f; // activation at note 68
      data.at(89) = 0.3f; // activation at note 89
      int sampleNumber = 145;
      ModelVector vec1{data, sampleNumber, Type::MergedInput};
      std::vector<ModelVector> output{vec1};

      writer.writeMidiOutput(output, buffer, bufferSize);

      REQUIRE(buffer.getNumEvents() == 3);
      int time;
      MidiMessage m;
      MidiBuffer::Iterator i(buffer);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(m.getFloatVelocity() == 1.0f);
      REQUIRE(time == 145);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 68);
      REQUIRE(m.getFloatVelocity() == 0.8f);
      REQUIRE(time == 145);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 89);
      REQUIRE(m.getFloatVelocity() == 0.3f);
      REQUIRE(time == 145);

      // check the samples played so far is correct
      REQUIRE(writer.playingMidiNotes.at(60) == 110);
      REQUIRE(writer.playingMidiNotes.at(68) == 110);
      REQUIRE(writer.playingMidiNotes.at(89) == 110);
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

      writer.writeMidiOutput(output, buffer, bufferSize);

      REQUIRE(buffer.getNumEvents() == 3);
      int time;
      MidiMessage m;
      MidiBuffer::Iterator i(buffer);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(m.getFloatVelocity() == 1.0f);
      REQUIRE(time == 145);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 33);
      REQUIRE(m.getFloatVelocity() == 0.27f);
      REQUIRE(time == 172);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 103);
      REQUIRE(m.getFloatVelocity() == 0.345f);
      REQUIRE(time == 239);

      // check the samples played so far is correct
      REQUIRE(writer.playingMidiNotes.at(60) == 110);
      REQUIRE(writer.playingMidiNotes.at(33) == 84);
      REQUIRE(writer.playingMidiNotes.at(103) == 16);
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

      writer.writeMidiOutput(output, buffer, bufferSize);

      REQUIRE(buffer.getNumEvents() == 6);
      int time;
      MidiMessage m;
      MidiBuffer::Iterator i(buffer);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(m.getFloatVelocity() == 1.0f);
      REQUIRE(time == 31);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 18);
      REQUIRE(m.getFloatVelocity() == 0.9f);
      REQUIRE(time == 31);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 33);
      REQUIRE(m.getFloatVelocity() == 0.27f);
      REQUIRE(time == 48);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 51);
      REQUIRE(m.getFloatVelocity() == 0.82f);
      REQUIRE(time == 48);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 103);
      REQUIRE(m.getFloatVelocity() == 0.97f);
      REQUIRE(time == 199);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 76);
      REQUIRE(m.getFloatVelocity() == 0.175f);
      REQUIRE(time == 199);

      // check the samples played so far is correct
      REQUIRE(writer.playingMidiNotes.at(60) == 224);
      REQUIRE(writer.playingMidiNotes.at(18) == 224);
      REQUIRE(writer.playingMidiNotes.at(33) == 207);
      REQUIRE(writer.playingMidiNotes.at(51) == 207);
      REQUIRE(writer.playingMidiNotes.at(103) == 56);
      REQUIRE(writer.playingMidiNotes.at(76) == 56);
    }
  }

  GIVEN("An instance of MidiOutputWriter with some notes playing") {
    MidiOutputWriter writer;
    MidiBuffer buffer;
    int bufferSize = 512;

    ModelVectorData data;
    data.at(60) = 1.0f; // activation at note 60
    data.at(64) = 0.9f; // activation at note 64
    data.at(67) = 0.5f; // activation at note 67
    int sampleNumber = 31;
    ModelVector vec{data, sampleNumber, Type::MergedInput};
    std::vector<ModelVector> output{vec};

    writer.writeMidiOutput(output, buffer, bufferSize);

    THEN("we parse Model output with a new note") {
      ModelVectorData newData;
      newData.at(70) = 0.3f; // activation at note 70
      int newSampleNumber = 158;
      ModelVector newVec{data, newSampleNumber, Type::MergedInput};
      std::vector<ModelVector> newOutput{newVec};

      writer.writeMidiOutput(newOutput, buffer, bufferSize);

      REQUIRE(buffer.getNumEvents() == 1);
      int time;
      MidiMessage m;
      MidiBuffer::Iterator i(buffer);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 70);
      REQUIRE(m.getFloatVelocity() == 0.3f);
      REQUIRE(time == 158);

      // check the samples played so far is correct
      REQUIRE(writer.playingMidiNotes.at(60) == 480);
      REQUIRE(writer.playingMidiNotes.at(64) == 480);
      REQUIRE(writer.playingMidiNotes.at(67) == 480);
      REQUIRE(writer.playingMidiNotes.at(70) == 97);
    }

    THEN("we parse Model output with a note that is already playing") {
      ModelVectorData newData;
      newData.at(60) = 0.3f; // activation at note 70
      int newSampleNumber = 12;
      ModelVector newVec{data, newSampleNumber, Type::MergedInput};
      std::vector<ModelVector> newOutput{newVec};

      writer.writeMidiOutput(newOutput, buffer, bufferSize);

      REQUIRE(buffer.getNumEvents() == 2);
      int time;
      MidiMessage m;
      MidiBuffer::Iterator i(buffer);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOff());
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(time == 12);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(m.getFloatVelocity() == 0.3f);
      REQUIRE(time == 12);

      // check the samples played so far is correct
      REQUIRE(writer.playingMidiNotes.at(60) == 243);
      REQUIRE(writer.playingMidiNotes.at(64) == 480);
      REQUIRE(writer.playingMidiNotes.at(67) == 480);
    }

    THEN("we parse Model output during a buffer where a note has reached the "
         "play limit") {
      writer.setMaxNoteLength(256);
      ModelVectorData newData;
      newData.at(60) = 0.3f; // activation at note 70
      int newSampleNumber = 61;
      ModelVector newVec{data, newSampleNumber, Type::MergedInput};
      std::vector<ModelVector> newOutput{newVec};

      writer.writeMidiOutput(newOutput, buffer, bufferSize);

      REQUIRE(buffer.getNumEvents() == 4);
      int time;
      MidiMessage m;
      MidiBuffer::Iterator i(buffer);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOff());
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(time == 31);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOff());
      REQUIRE(m.getNoteNumber() == 64);
      REQUIRE(time == 31);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOff());
      REQUIRE(m.getNoteNumber() == 67);
      REQUIRE(time == 31);

      i.getNextEvent(m, time);
      REQUIRE(m.isNoteOn());
      REQUIRE(m.getNoteNumber() == 60);
      REQUIRE(m.getFloatVelocity() == 0.3f);
      REQUIRE(time == 61);

      // check the samples played so far is correct
      REQUIRE(writer.playingMidiNotes.at(60) == 194);
      REQUIRE(writer.playingMidiNotes.at(64) == 0);
      REQUIRE(writer.playingMidiNotes.at(67) == 0);
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

      sampleNumModuloBufferSize(modelOutput, bufferSize);

      REQUIRE(modelOutput.at(0).sampleNumber == 49);
      REQUIRE(modelOutput.at(1).sampleNumber == 192);
      REQUIRE(modelOutput.at(2).sampleNumber == 211);
    }

    THEN("with buffer size of 512") {
      int bufferSize = 512;

      sampleNumModuloBufferSize(modelOutput, bufferSize);

      REQUIRE(modelOutput.at(0).sampleNumber == 305);
      REQUIRE(modelOutput.at(1).sampleNumber == 448);
      REQUIRE(modelOutput.at(2).sampleNumber == 467);
    }
  }
}
