/*
 * MidiProcessor.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiProcessor.hpp"
#include <catch2/catch.hpp>

SCENARIO("MidiProcessor") {
  GIVEN("an instance of MidiProcessor, neurons: 3") {
    MidiProcessor processor(3);

    // Test default values
    REQUIRE(processor.get_midi_map() == std::vector<int>{1, 1, 1});
    REQUIRE(processor.get_global_volume() == Approx(1.0f));

    REQUIRE(processor.get_volume_clip_min() == 1);
    REQUIRE(processor.get_volume_clip_max() == 127);
    REQUIRE(processor.get_volume_clip_range() == 126);

    REQUIRE(processor.get_note_at(0) == 1);
    REQUIRE(processor.get_note_at(1) == 1);
    REQUIRE(processor.get_note_at(2) == 1);

    WHEN("we set the global volume") {
      processor.set_global_volume(0.69f);

      REQUIRE(processor.get_global_volume() == Approx(0.69f));
    }

    WHEN("we set the volume clip range") {
      processor.set_volume_clip(52, 100);

      REQUIRE(processor.get_volume_clip_min() == 52);
      REQUIRE(processor.get_volume_clip_max() == 100);
      REQUIRE(processor.get_volume_clip_range() == 48);
    }

    WHEN("we set the MIDI notes") {
      processor.set_note_at(0, 25);
      processor.set_note_at(1, 89);
      processor.set_note_at(2, 111);

      REQUIRE(processor.get_note_at(0) == 25);
      REQUIRE(processor.get_note_at(1) == 89);
      REQUIRE(processor.get_note_at(2) == 111);
    }
  }
}

SCENARIO("MidiProcessor MIDI Volume") {
  GIVEN("an instance of MidiProcessor, neurons: 3") {

    MidiProcessor processor(3);

    WHEN("we clip the brain output") {
      REQUIRE(processor.clip_brain_output(0) == 0);
      REQUIRE(processor.clip_brain_output(1) == 1);
      REQUIRE(processor.clip_brain_output(2) == 1);
    }

    WHEN("we get the volume as a percentage of the max volume") {
      REQUIRE(processor.as_percent_of_max(0) == Approx(0.0f));
      REQUIRE(processor.as_percent_of_max(1) == Approx(1.0f));
    }

    WHEN("we clip the MIDI volume, clip: (1, 127)") {
      REQUIRE(processor.get_clipped_midi_volume(1.0f) == 127);
      REQUIRE(processor.get_clipped_midi_volume(0.8f) == 102);
      REQUIRE(processor.get_clipped_midi_volume(0.0f) == 0);
    }

    WHEN("we clip the MIDI volume, clip: (50, 100)") {
      processor.set_volume_clip(50, 100);
      REQUIRE(processor.get_clipped_midi_volume(1.0f) == 100);
      REQUIRE(processor.get_clipped_midi_volume(0.8f) == 90);
      REQUIRE(processor.get_clipped_midi_volume(0.0f) == 0);
    }

    WHEN("we get the note velocity, clip: (1, 127)") {
      REQUIRE(processor.get_note_velocity(0) == 0);
      REQUIRE(processor.get_note_velocity(1) == 127);
    }

    WHEN("we get the note velocity, clip: (50, 80)") {
      processor.set_volume_clip(50, 80);

      REQUIRE(processor.get_note_velocity(0) == 0);
      REQUIRE(processor.get_note_velocity(1) == 80);
    }

    WHEN("we get the note velocity, clip: (50, 80), global volume: 0.5") {
      processor.set_volume_clip(50, 80);
      processor.set_global_volume(0.5f);

      REQUIRE(processor.get_note_velocity(0) == 0);
      REQUIRE(processor.get_note_velocity(1) == 40);
    }
  }
}

SCENARIO("MidiProcessor Adding and Removing Notes") {
  GIVEN("an instance of MidiProcessor, neurons: 3") {
    MidiProcessor processor(3);

    WHEN("we set the note values") {
      processor.set_note_at(0, 60);
      processor.set_note_at(1, 64);
      processor.set_note_at(2, 67);

      REQUIRE(processor.get_midi_map() == std::vector<int>{60, 64, 67});

      THEN("we add two notes") {
        processor.add_midi_note(60);
        REQUIRE(processor.get_midi_map() == std::vector<int>{60, 64, 67, 60});

        processor.add_midi_note(8);
        REQUIRE(processor.get_midi_map() ==
                std::vector<int>{60, 64, 67, 60, 8});

        THEN("we remove three notes") {
          processor.remove_midi_note();
          REQUIRE(processor.get_midi_map() == std::vector<int>{60, 64, 67, 60});

          processor.remove_midi_note_at(0);
          REQUIRE(processor.get_midi_map() == std::vector<int>{64, 67, 60});

          processor.remove_midi_note_at(1);
          REQUIRE(processor.get_midi_map() == std::vector<int>{64, 60});
        }
      }
    }
  }
}

SCENARIO("MidiProcessor Buffer Rendering") {
  GIVEN("an instance of MidiProcessor, neurons: 3") {
    MidiProcessor processor(3);
    MidiBuffer buffer;
    processor.set_note_at(0, 60);
    processor.set_note_at(1, 64);
    processor.set_note_at(2, 67);

    WHEN("we render a buffer, output: (1, 1, 1), sample_number: 100") {
      std::vector<int> output = std::vector<int>{1, 1, 1};
      int sample_number = 100;
      processor.render_buffer(buffer, output, sample_number);

      REQUIRE(buffer.getNumEvents() == 3);
      REQUIRE(buffer.getFirstEventTime() == 100);
    }

    WHEN("we render a buffer, output: (1, 0, 1), sample_number: 167") {
      std::vector<int> output = std::vector<int>{1, 0, 1};
      int sample_number = 167;
      processor.render_buffer(buffer, output, sample_number);

      REQUIRE(buffer.getNumEvents() == 2);
      REQUIRE(buffer.getFirstEventTime() == 167);
    }

    WHEN("we render a buffer, output: (1, 0, 0), sample_number: 17") {
      std::vector<int> output = std::vector<int>{1, 0, 0};
      int sample_number = 17;
      processor.render_buffer(buffer, output, sample_number);

      REQUIRE(buffer.getNumEvents() == 1);
      REQUIRE(buffer.getFirstEventTime() == 17);

      int time;
      MidiMessage m;
      for (MidiBuffer::Iterator i(buffer); i.getNextEvent(m, time);) {
        REQUIRE(m.getNoteNumber() == 60);
        REQUIRE(m.getVelocity() == 127);
      }
    }

    WHEN("we render a buffer, output: (0, 1, 0), sample_number: 45") {
      std::vector<int> output = std::vector<int>{0, 1, 0};
      int sample_number = 45;
      processor.render_buffer(buffer, output, sample_number);

      REQUIRE(buffer.getNumEvents() == 1);
      REQUIRE(buffer.getFirstEventTime() == 45);

      int time;
      MidiMessage m;
      for (MidiBuffer::Iterator i(buffer); i.getNextEvent(m, time);) {
        REQUIRE(m.getNoteNumber() == 64);
        REQUIRE(m.getVelocity() == 127);
      }
    }

    WHEN("we render a buffer, output: (0, 0, 1), sample_number: 229") {
      std::vector<int> output = std::vector<int>{0, 0, 1};
      int sample_number = 229;
      processor.render_buffer(buffer, output, sample_number);

      REQUIRE(buffer.getNumEvents() == 1);
      REQUIRE(buffer.getFirstEventTime() == 229);

      int time;
      MidiMessage m;
      for (MidiBuffer::Iterator i(buffer); i.getNextEvent(m, time);) {
        REQUIRE(m.getNoteNumber() == 67);
        REQUIRE(m.getVelocity() == 127);
      }
    }
  }
}
