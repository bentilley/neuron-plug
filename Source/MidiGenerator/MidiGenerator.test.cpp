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
