/*
 * MidiProcessor.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiProcessor.hpp"

class MidiProcessorTests : public UnitTest {
public:
  MidiProcessorTests() : UnitTest("MidiProcessor Testing") {}

  void runTest() override {
    MidiProcessor processor;

    // == Instantiation ==
    beginTest("Instantiation");

    expect(processor.get_midi_map() == std::vector<int>{60, 64, 67},
           "MidiProcessor missing default `midi_map`");

    // == render_buffer ==
    beginTest("render_buffer");

    std::vector<int> output{1, 1, 1};
    MidiBuffer buffer;
    int sample_number = 100;
    processor.render_buffer(buffer, output, sample_number);

    expect(buffer.getNumEvents() == 3, "Wrong number of MIDI events");
    expect(buffer.getFirstEventTime() == 100,
           "MIDI events have wrong sample number");

    output = std::vector<int>{1, 0, 1};
    buffer = MidiBuffer();
    sample_number = 167;
    processor.render_buffer(buffer, output, sample_number);

    expect(buffer.getNumEvents() == 2, "Wrong number of MIDI events");
    expect(buffer.getFirstEventTime() == 167,
           "MIDI events have wrong sample number");

    // == render_buffer note numbers==
    beginTest("render_buffer note numbers");

    output = std::vector<int>{1, 0, 0};
    buffer = MidiBuffer();
    sample_number = 17;
    processor.render_buffer(buffer, output, sample_number);

    expect(buffer.getNumEvents() == 1, "Wrong number of MIDI events");
    expect(buffer.getFirstEventTime() == 17,
           "MIDI events have wrong sample number");

    int time;
    MidiMessage m;
    for (MidiBuffer::Iterator i(buffer); i.getNextEvent(m, time);) {
      expect(m.getNoteNumber() == 60, "note number is incorrect");
    }

    output = std::vector<int>{0, 1, 0};
    buffer = MidiBuffer();
    sample_number = 45;
    processor.render_buffer(buffer, output, sample_number);

    expect(buffer.getNumEvents() == 1, "Wrong number of MIDI events");
    expect(buffer.getFirstEventTime() == 45,
           "MIDI events have wrong sample number");

    for (MidiBuffer::Iterator i(buffer); i.getNextEvent(m, time);) {
      expect(m.getNoteNumber() == 64, "note number is incorrect");
    }

    output = std::vector<int>{0, 0, 1};
    buffer = MidiBuffer();
    sample_number = 229;
    processor.render_buffer(buffer, output, sample_number);

    expect(buffer.getNumEvents() == 1, "Wrong number of MIDI events");
    expect(buffer.getFirstEventTime() == 229,
           "MIDI events have wrong sample number");

    for (MidiBuffer::Iterator i(buffer); i.getNextEvent(m, time);) {
      expect(m.getNoteNumber() == 67, "note number is incorrect");
    }
  };
};

static MidiProcessorTests test;
