/*
 * MidiInputTransformer.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiInputTransformer.hpp"
#include <catch2/catch.hpp>

SCENARIO("MidiInputTransformer") {
  GIVEN("an instance of MidiInputTransformer and a MidiBuffer") {
    MidiInputTransformer transformer;
    MidiBuffer buffer;

    WHEN("there are no MIDI events in the MidiBuffer") {
      auto output{transformer.parseMidiInput(buffer)};
      printf("womble\n");
      REQUIRE(output.size() == 0);
    }

    WHEN("there is a note ON event in the MidiBuffer") {
      // addEvent( noteOn(channel, noteNumber, velocity), sampleNumber )
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)100), 52344);

      auto output{transformer.parseMidiInput(buffer)};

      REQUIRE(output.size() == 1);

      ModelInput res{output.at(0)};
      REQUIRE(res.data.at(65) == Approx(100.0f / 127.0f));
      REQUIRE(res.sampleNumber == 52344);
      REQUIRE(res.inputType == ModelInputType::MidiInput);
    }

    WHEN("there is a note OFF event in the MidiBuffer") {
      // addEvent( noteOn(channel, noteNumber, velocity), sampleNumber )
      buffer.addEvent(MidiMessage::noteOff(1, 65, (uint8)100), 52344);

      auto output{transformer.parseMidiInput(buffer)};

      REQUIRE(output.size() == 0); // ignore note OFF events
    }

    WHEN("there are multiple note ON events in the MidiBuffer") {
      // addEvent( noteOn(channel, noteNumber, velocity), sampleNumber )
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)100), 52344);
      buffer.addEvent(MidiMessage::noteOn(1, 111, (uint8)67), 52418);

      auto output{transformer.parseMidiInput(buffer)};

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

    WHEN("there are a mixture of note ON and note OFF events in the "
         "MidiBuffer") {
      // addEvent( noteOn(channel, noteNumber, velocity), sampleNumber )
      buffer.addEvent(MidiMessage::noteOn(1, 65, (uint8)100), 52344);
      buffer.addEvent(MidiMessage::noteOn(1, 111, (uint8)67), 52418);
      buffer.addEvent(MidiMessage::noteOff(1, 68, (uint8)100), 52349);

      auto output{transformer.parseMidiInput(buffer)};

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

      REQUIRE_THROWS(
          transformer.midiMessageToModelInput(message, sampleNumber));
    }
  }
}
