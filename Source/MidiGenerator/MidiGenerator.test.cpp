/*
 * MidiGenerator.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiGenerator.hpp"

class MidiGeneratorTests : public UnitTest {
public:
  MidiGeneratorTests() : UnitTest("MidiGenerator Testing") {}

  void runTest() override {
    MidiGenerator generator;

    // == Instantiation ==
    beginTest("Instantiation");

    expect(!generator.get_is_on(), "midi generator default on state wrong");

    // == Toggling Settings ==
    beginTest("toggleOnOff");

    generator.toggleOnOff();
    expect(generator.get_is_on(), "midi generator should have been on");

    generator.toggleOnOff();
    expect(!generator.get_is_on(), "midi generator should have been off");

    beginTest("toggleReceivesMidi");

    // the component can't currently process midi so this should always be false
    generator.toggleReceivesMidi();
    expect(!generator.get_receives_midi(),
           "midi generator should not receive midi");

    generator.toggleReceivesMidi();
    expect(!generator.get_receives_midi(),
           "midi generator should not receive midi");

    // == getters and setters ==
    beginTest("subdivision - get / set");

    expect(generator.get_subdivision() == 1,
           "default subdivision is wrong value");
    generator.set_subdivision(8);
    expect(generator.get_subdivision() == 8,
           "subdivision was not set correctly");
    generator.set_subdivision(2);
    expect(generator.get_subdivision() == 2,
           "subdivision was not set correctly");

    beginTest("volume - get / set");

    expectWithinAbsoluteError<float>(generator.get_volume(), 1.0, 0.01,
                                     "default volume is wrong value");
    generator.set_volume(0.5);
    expectWithinAbsoluteError<float>(generator.get_volume(), 0.5, 0.01,
                                     "volume was not set correctly");
    generator.set_volume(0.77);
    expectWithinAbsoluteError<float>(generator.get_volume(), 0.77, 0.01,
                                     "volume was not set correctly");

    expect(generator.get_volume_clip_min() == 1, "default volume clip wrong");
    expect(generator.get_volume_clip_max() == 127, "default volume clip wrong");

    generator.set_volume_clip(15, 100);
    expect(generator.get_volume_clip_min() == 15, "vol clip should be 15");
    expect(generator.get_volume_clip_max() == 100, "vol clip should be 100");

    generator.set_volume_clip(1, 45);
    expect(generator.get_volume_clip_min() == 1, "vol clip should be 1");
    expect(generator.get_volume_clip_max() == 45, "vol clip should be 45");

    // == generate_next_midi_buffer ==
    beginTest("generate_next_midi_buffer");

    MidiBuffer buffer;
    double sample_rate{44100};
    int num_samples{64};
    AudioPlayHead::CurrentPositionInfo pos;
    pos.bpm = 100;
    pos.timeInSamples = 0;

    generator.generate_next_midi_buffer(buffer, pos, sample_rate, num_samples);

    expect(buffer.getNumEvents() == 3, "Wrong number of MIDI events");
    expect(buffer.getFirstEventTime() == 0,
           "MIDI events have wrong sample number");

    std::vector<int> expected_notes{60, 64, 67};
    int j{0};
    int time;
    MidiMessage m;
    for (MidiBuffer::Iterator i(buffer); i.getNextEvent(m, time); ++j) {
      expect(m.getNoteNumber() == expected_notes.at(j),
             "note number is incorrect");
    }
  };
};

static MidiGeneratorTests test;
