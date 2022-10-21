/*
 * MidiGenerator.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <catch2/catch.hpp>

#define private public
#include "MidiGenerator.hpp"

SCENARIO("MidiGenerator")
{
  GIVEN("an instance of MidiGenerator, neurons: 3")
  {
    MidiGenerator generator(3);

    THEN("the default values are correct")
    {
      REQUIRE(!generator.getIsOn());
      // Parameters
      REQUIRE(generator.get_subdivision() == 1);
      REQUIRE(generator.get_volume() == Approx(1.0f));
      REQUIRE(generator.get_volume_clip_min() == 0.0);
      REQUIRE(generator.get_volume_clip_max() == 1.0);
      // MIDI Notes
      REQUIRE(generator.get_neuron_midi_note(0) == 1);
      REQUIRE(generator.get_neuron_midi_note(1) == 1);
      REQUIRE(generator.get_neuron_midi_note(2) == 1);
      // Input Weights
      REQUIRE(generator.get_neuron_input_weight(0) == 0);
      REQUIRE(generator.get_neuron_input_weight(1) == 0);
      REQUIRE(generator.get_neuron_input_weight(2) == 0);
      // Thresholds
      REQUIRE(generator.get_neuron_threshold(0) == 0);
      REQUIRE(generator.get_neuron_threshold(1) == 0);
      REQUIRE(generator.get_neuron_threshold(2) == 0);
      // Connection Weights
      REQUIRE(generator.get_neuron_connection_weight(0, 0) == 0);
      REQUIRE(generator.get_neuron_connection_weight(0, 1) == 0);
      REQUIRE(generator.get_neuron_connection_weight(0, 2) == 0);
      REQUIRE(generator.get_neuron_connection_weight(1, 0) == 0);
      REQUIRE(generator.get_neuron_connection_weight(1, 1) == 0);
      REQUIRE(generator.get_neuron_connection_weight(1, 2) == 0);
      REQUIRE(generator.get_neuron_connection_weight(2, 0) == 0);
      REQUIRE(generator.get_neuron_connection_weight(2, 1) == 0);
      REQUIRE(generator.get_neuron_connection_weight(2, 2) == 0);
    }

    WHEN("we toggle the on/off state")
    {
      generator.toggleOnOff();
      REQUIRE(generator.getIsOn());

      generator.toggleOnOff();
      REQUIRE(!generator.getIsOn());
    }

    WHEN("we toggle the beat clock")
    {
      generator.toggleBeatClockIsOn();
      REQUIRE(!generator.getBeatClockIsOn());

      generator.toggleBeatClockIsOn();
      REQUIRE(generator.getBeatClockIsOn());
    }

    WHEN("we toggle the MIDI input")
    {
      generator.toggleMidiInputIsOn();
      REQUIRE(!generator.getMidiInputIsOn());

      generator.toggleMidiInputIsOn();
      REQUIRE(generator.getMidiInputIsOn());
    }

    WHEN("we set the parameters")
    {
      generator.set_subdivision(8);
      REQUIRE(generator.get_subdivision() == 8);

      generator.set_subdivision(2);
      REQUIRE(generator.get_subdivision() == 2);

      generator.set_volume(0.5);
      REQUIRE(generator.get_volume() == Approx(0.5f));

      generator.set_volume(0.77);
      REQUIRE(generator.get_volume() == Approx(0.77f));

      generator.set_volume_clip(0.1, 0.8);
      REQUIRE(generator.get_volume_clip_min() == Approx(0.1));
      REQUIRE(generator.get_volume_clip_max() == Approx(0.8));

      generator.set_volume_clip(0.01, 0.385);
      REQUIRE(generator.get_volume_clip_min() == Approx(0.01));
      REQUIRE(generator.get_volume_clip_max() == Approx(0.385));
    }

    WHEN("we set the MIDI notes")
    {
      generator.set_neuron_midi_note(0, 34);
      generator.set_neuron_midi_note(1, 74);
      generator.set_neuron_midi_note(2, 100);

      REQUIRE(generator.get_neuron_midi_note(0) == 34);
      REQUIRE(generator.get_neuron_midi_note(1) == 74);
      REQUIRE(generator.get_neuron_midi_note(2) == 100);
    }

    WHEN("we set the neuron input weights")
    {
      generator.set_neuron_input_weight(0, 18);
      generator.set_neuron_input_weight(1, 5);
      generator.set_neuron_input_weight(2, 103);

      REQUIRE(generator.get_neuron_input_weight(0) == 18);
      REQUIRE(generator.get_neuron_input_weight(1) == 5);
      REQUIRE(generator.get_neuron_input_weight(2) == 103);
    }

    WHEN("we set the neuron thresholds")
    {
      generator.set_neuron_threshold(0, 8);
      generator.set_neuron_threshold(1, 52);
      generator.set_neuron_threshold(2, 10);

      REQUIRE(generator.get_neuron_threshold(0) == 8);
      REQUIRE(generator.get_neuron_threshold(1) == 52);
      REQUIRE(generator.get_neuron_threshold(2) == 10);
    }

    WHEN("we set the neuron connection weights")
    {

      generator.set_neuron_connection_weight(0, 0, 18);
      generator.set_neuron_connection_weight(0, 1, -3);
      generator.set_neuron_connection_weight(0, 2, 8);
      generator.set_neuron_connection_weight(1, 0, 13);
      generator.set_neuron_connection_weight(1, 1, 38);
      generator.set_neuron_connection_weight(1, 2, -8);
      generator.set_neuron_connection_weight(2, 0, -58);
      generator.set_neuron_connection_weight(2, 1, 1);
      generator.set_neuron_connection_weight(2, 2, 180);

      REQUIRE(generator.get_neuron_connection_weight(0, 0) == 18);
      REQUIRE(generator.get_neuron_connection_weight(0, 1) == -3);
      REQUIRE(generator.get_neuron_connection_weight(0, 2) == 8);
      REQUIRE(generator.get_neuron_connection_weight(1, 0) == 13);
      REQUIRE(generator.get_neuron_connection_weight(1, 1) == 38);
      REQUIRE(generator.get_neuron_connection_weight(1, 2) == -8);
      REQUIRE(generator.get_neuron_connection_weight(2, 0) == -58);
      REQUIRE(generator.get_neuron_connection_weight(2, 1) == 1);
      REQUIRE(generator.get_neuron_connection_weight(2, 2) == 180);
    }
  }
}

SCENARIO("MidiGenerator Network Methods")
{
  GIVEN("an instance of MidiGenerator, neurons: 3")
  {
    MidiGenerator generator(3);

    REQUIRE(generator.num_neurons() == 3);

    WHEN("we add 2 neurons")
    {
      generator.add_neuron();
      generator.add_neuron();

      REQUIRE(generator.num_neurons() == 5);

      REQUIRE(generator.get_neuron_connection_weight(3, 0) == 0);
      REQUIRE(generator.get_neuron_connection_weight(3, 1) == 0);
      REQUIRE(generator.get_neuron_connection_weight(3, 2) == 0);
      REQUIRE(generator.get_neuron_connection_weight(3, 3) == 0);
      REQUIRE(generator.get_neuron_connection_weight(3, 4) == 0);

      REQUIRE(generator.get_neuron_connection_weight(4, 0) == 0);
      REQUIRE(generator.get_neuron_connection_weight(4, 1) == 0);
      REQUIRE(generator.get_neuron_connection_weight(4, 2) == 0);
      REQUIRE(generator.get_neuron_connection_weight(4, 3) == 0);
      REQUIRE(generator.get_neuron_connection_weight(4, 4) == 0);

      REQUIRE(generator.get_neuron_connection_weight(0, 3) == 0);
      REQUIRE(generator.get_neuron_connection_weight(0, 4) == 0);
      REQUIRE(generator.get_neuron_connection_weight(1, 3) == 0);
      REQUIRE(generator.get_neuron_connection_weight(1, 4) == 0);
      REQUIRE(generator.get_neuron_connection_weight(2, 3) == 0);
      REQUIRE(generator.get_neuron_connection_weight(2, 4) == 0);

      THEN("we remove 2 neurons")
      {

        generator.remove_neuron();
        generator.remove_neuron();

        REQUIRE(generator.num_neurons() == 3);
      }
    }
  }

  GIVEN("an instance of MidiGenerator with connection weights, neurons: 3")
  {
    MidiGenerator generator(3);
    generator.set_neuron_connection_weight(0, 0, 1);
    generator.set_neuron_connection_weight(0, 1, 2);
    generator.set_neuron_connection_weight(0, 2, 3);
    generator.set_neuron_connection_weight(1, 0, 4);
    generator.set_neuron_connection_weight(1, 1, 5);
    generator.set_neuron_connection_weight(1, 2, 6);
    generator.set_neuron_connection_weight(2, 0, 7);
    generator.set_neuron_connection_weight(2, 1, 8);
    generator.set_neuron_connection_weight(2, 2, 9);

    WHEN("we remove neuron 0")
    {
      generator.remove_neuron_at(0);

      REQUIRE(generator.get_neuron_connection_weight(0, 0) == 5);
      REQUIRE(generator.get_neuron_connection_weight(0, 1) == 6);
      REQUIRE(generator.get_neuron_connection_weight(1, 0) == 8);
      REQUIRE(generator.get_neuron_connection_weight(1, 1) == 9);

      THEN("we remove neuron 1")
      {
        generator.remove_neuron_at(1);

        REQUIRE(generator.get_neuron_connection_weight(0, 0) == 5);
      }
    }
  }

  GIVEN("an instance of MidiGenerator with parameters setup, neurons: 3")
  {
    MidiGenerator generator(3);

    generator.toggleOnOff();

    generator.set_neuron_midi_note(0, 60);
    generator.set_neuron_midi_note(1, 64);
    generator.set_neuron_midi_note(2, 67);

    generator.set_neuron_input_weight(0, 1);
    generator.set_neuron_input_weight(1, 1);
    generator.set_neuron_input_weight(2, 1);

    generator.set_neuron_connection_weight(0, 0, -5);
    generator.set_neuron_connection_weight(0, 1, 2);
    generator.set_neuron_connection_weight(0, 2, 1);
    generator.set_neuron_connection_weight(1, 0, 1);
    generator.set_neuron_connection_weight(1, 1, -6);
    generator.set_neuron_connection_weight(1, 2, 2);
    generator.set_neuron_connection_weight(2, 0, 2);
    generator.set_neuron_connection_weight(2, 1, 1);
    generator.set_neuron_connection_weight(2, 2, -7);

    WHEN("we generate a midi buffer")
    {
      MidiBuffer input, output;
      SystemInfo sysInfo(44100, 64);
      AudioPlayHead::CurrentPositionInfo pos;
      pos.bpm = 100;
      pos.timeInSamples = 0;

      generator.generate_next_midi_buffer(input, output, pos, sysInfo);

      REQUIRE(output.getNumEvents() == 3);
      REQUIRE(output.getFirstEventTime() == 0);

      std::vector<int> expected_notes{60, 64, 67};
      int j{0};
      int time;
      MidiMessage m;
      for (MidiBuffer::Iterator i(output); i.getNextEvent(m, time); ++j) {
        REQUIRE(m.getNoteNumber() == expected_notes.at(j));
      }
    }
  }
}
