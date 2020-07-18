/*
 * MidiInputTransformer.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <catch2/catch.hpp>

#define private public
#include "MidiInputTransformer.hpp"

SCENARIO("MidiInputTransformer") {
  GIVEN("an instance of MidiInputTransformer and a MidiBuffer") {
    MidiInputTransformer transformer;
    MidiBuffer buffer;

    WHEN("there are no MIDI events in the MidiBuffer") {
      auto output{transformer.getModelInputForBuffer(buffer)};
      REQUIRE(output.size() == 0);
    }

    WHEN("there is a note ON event in the MidiBuffer") {
      // addEvent( noteOn(channel, noteNumber, velocity), sampleNumber )
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)100), 52344);

      auto output{transformer.getModelInputForBuffer(buffer)};

      REQUIRE(output.size() == 1);

      ModelInput res{output.at(0)};
      REQUIRE(res.data.at(65) == Approx(100.0f / 127.0f));
      REQUIRE(res.sampleNumber == 52344);
      REQUIRE(res.inputType == ModelInputType::MidiInput);
    }

    WHEN("there is a note OFF event in the MidiBuffer") {
      // addEvent( noteOn(channel, noteNumber, velocity), sampleNumber )
      buffer.addEvent(MidiMessage::noteOff(1, 65, (uint8)100), 52344);

      auto output{transformer.getModelInputForBuffer(buffer)};

      REQUIRE(output.size() == 0); // ignore note OFF events
    }

    WHEN("there are multiple note ON events in the MidiBuffer") {
      // addEvent( noteOn(channel, noteNumber, velocity), sampleNumber )
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)100), 52344);
      buffer.addEvent(MidiMessage::noteOn(1, 111, (uint8)67), 52418);

      auto output{transformer.getModelInputForBuffer(buffer)};

      REQUIRE(output.size() == 2);

      ModelInput input1{output.at(0)};
      REQUIRE(input1.data.at(65) == Approx(100.0f / 127.0f));
      REQUIRE(input1.sampleNumber == 52344);
      REQUIRE(input1.inputType == ModelInputType::MidiInput);

      ModelInput input2{output.at(1)};
      REQUIRE(input2.data.at(111) == Approx(67.0f / 127.0f));
      REQUIRE(input2.sampleNumber == 52418);
      REQUIRE(input2.inputType == ModelInputType::MidiInput);
    }

    WHEN("there are multiple note ON events in the MidiBuffer with the same "
         "sample") {
      // addEvent( noteOn(channel, noteNumber, velocity), sampleNumber )
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)100), 52344);
      buffer.addEvent(MidiMessage::noteOn(1, 111, (uint8)67), 52344);
      buffer.addEvent(MidiMessage::noteOn(1, 78, (uint8)18), 52344);

      auto output{transformer.getModelInputForBuffer(buffer)};

      REQUIRE(output.size() == 1);

      ModelInput input1{output.at(0)};
      REQUIRE(input1.data.at(65) == Approx(100.0f / 127.0f));
      REQUIRE(input1.data.at(111) == Approx(67.0f / 127.0f));
      REQUIRE(input1.data.at(78) == Approx(18.0f / 127.0f));
      REQUIRE(input1.sampleNumber == 52344);
      REQUIRE(input1.inputType == ModelInputType::MidiInput);
    }

    WHEN("there are multiple note ON events in the MidiBuffer with the same "
         "sample and MIDI note") {
      // addEvent( noteOn(channel, noteNumber, velocity), sampleNumber )
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)100), 7811643);
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)67), 7811643);
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)18), 7811643);

      auto output{transformer.getModelInputForBuffer(buffer)};

      REQUIRE(output.size() == 1);

      ModelInput input1{output.at(0)};
      REQUIRE(input1.data.at(65) == Approx((100.0f + 67.0f + 18.0f) / 127.0f));
      REQUIRE(input1.sampleNumber == 7811643);
      REQUIRE(input1.inputType == ModelInputType::MidiInput);
    }

    WHEN("there are a mixture of note ON and note OFF events in the "
         "MidiBuffer") {
      // addEvent( noteOn(channel, noteNumber, velocity), sampleNumber )
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)100), 52344);
      buffer.addEvent(MidiMessage::noteOn(1, 111, (uint8)67), 52418);
      buffer.addEvent(MidiMessage::noteOff(1, 68, (uint8)100), 52349);

      auto output{transformer.getModelInputForBuffer(buffer)};

      REQUIRE(output.size() == 2);

      ModelInput input1{output.at(0)};
      REQUIRE(input1.data.at(65) == Approx(100.0f / 127.0f));
      REQUIRE(input1.sampleNumber == 52344);
      REQUIRE(input1.inputType == ModelInputType::MidiInput);

      ModelInput input2{output.at(1)};
      REQUIRE(input2.data.at(111) == Approx(67.0f / 127.0f));
      REQUIRE(input2.sampleNumber == 52418);
      REQUIRE(input2.inputType == ModelInputType::MidiInput);
    }

    WHEN("there are a complex mixture of note ON and note OFF events in the "
         "MidiBuffer") {
      // addEvent( noteOn(channel, noteNumber, velocity), sampleNumber )
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)100), 11453);
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)67), 11453);
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)18), 11453);
      buffer.addEvent(MidiMessage::noteOn(1, 111, (uint8)67), 11566);
      buffer.addEvent(MidiMessage::noteOff(1, 68, (uint8)100), 11566);
      buffer.addEvent(MidiMessage::noteOn(1, 12, (uint8)99), 11566);
      buffer.addEvent(MidiMessage::noteOff(1, 68, (uint8)111), 11589);

      auto output{transformer.getModelInputForBuffer(buffer)};

      REQUIRE(output.size() == 2);

      ModelInput input1{output.at(0)};
      REQUIRE(input1.data.at(65) == Approx((100.0f + 67.0f + 18.0f) / 127.0f));
      REQUIRE(input1.sampleNumber == 11453);
      REQUIRE(input1.inputType == ModelInputType::MidiInput);

      ModelInput input2{output.at(1)};
      REQUIRE(input2.data.at(111) == Approx(67.0f / 127.0f));
      REQUIRE(input2.data.at(12) == Approx(99.0f / 127.0f));
      REQUIRE(input2.sampleNumber == 11566);
      REQUIRE(input2.inputType == ModelInputType::MidiInput);
    }
  }
}

SCENARIO("MidiInputTransformer - midiMessageToModelInput") {
  GIVEN("an instance of MidiInputTransformer") {
    MidiInputTransformer transformer;

    WHEN("we parse a note ON MIDI event") {
      MidiMessage message{MidiMessage::noteOn(1, 23, (uint8)120)};
      int sampleNumber{7719};

      ModelInput result{
          transformer.midiMessageToModelInput(message, sampleNumber)};

      REQUIRE(result.data.at(23) == Approx(120.0f / 127.0f));
      REQUIRE(result.sampleNumber == 7719);
      REQUIRE(result.inputType == ModelInputType::MidiInput);
    }

    WHEN("we parse a note OFF MIDI event") {
      MidiMessage message{MidiMessage::noteOff(1, 23, (uint8)120)};
      int sampleNumber{7719};

      REQUIRE_THROWS_WITH(
          transformer.midiMessageToModelInput(message, sampleNumber),
          Catch::Contains("MIDI message is not noteOn"));
    }
  }
}
