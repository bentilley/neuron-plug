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
    MidiProcessor processor(3);

    // == Instantiation ==
    beginTest("Instantiation");

    expect(processor.get_midi_map() == std::vector<int>{1, 1, 1},
           "MidiProcessor missing default `midi_map`");

    // == Getters & Setters
    beginTest("global volume");

    expectWithinAbsoluteError<float>(processor.get_global_volume(), 1.0f, 0.01f,
                                     "default global volume is wrong");
    processor.set_global_volume(0.5f);
    expectWithinAbsoluteError<float>(processor.get_global_volume(), 0.5f, 0.01f,
                                     "global volume was not set correctly");
    processor.set_global_volume(0.69f);
    expectWithinAbsoluteError<float>(processor.get_global_volume(), 0.69f,
                                     0.01f,
                                     "global volume was not set correctly");
    processor.set_global_volume(1.0f);

    beginTest("volume clip range");

    expect(processor.get_volume_clip_min() == 1, "wrong default vol clip");
    expect(processor.get_volume_clip_max() == 127, "wrong default vol clip");
    expect(processor.get_volume_clip_range() == 126, "wrong default vol clip");

    processor.set_volume_clip(52, 100);
    expect(processor.get_volume_clip_min() == 52, "wrong vol clip set");
    expect(processor.get_volume_clip_max() == 100, "wrong vol clip set");
    expect(processor.get_volume_clip_range() == 48, "wrong vol clip set");
    processor.set_volume_clip(1, 127);

    // == MIDI Notes ==
    beginTest("MIDI Notes");

    expect(processor.get_note_at(0) == 1, "1st default note is 1");
    expect(processor.get_note_at(1) == 1, "2nd default note is 1");
    expect(processor.get_note_at(2) == 1, "3rd default note is 1");

    processor.set_note_at(0, 25);
    processor.set_note_at(1, 89);
    processor.set_note_at(2, 111);

    expect(processor.get_note_at(0) == 25, "new note is 25");
    expect(processor.get_note_at(1) == 89, "new note is 89");
    expect(processor.get_note_at(2) == 111, "new note is 111");

    // == Clip Midi Volume ==
    beginTest("clip_brain_output");

    expect(processor.clip_brain_output(0) == 0, "wrong clipped brain output 0");
    expect(processor.clip_brain_output(1) == 1, "wrong clipped brain output 1");
    expect(processor.clip_brain_output(2) == 1, "wrong clipped brain output 2");

    beginTest("as_percent_of_max");

    expectWithinAbsoluteError<float>(processor.as_percent_of_max(0), 0.0f,
                                     0.01f,
                                     "incorrect output percentage for 0");
    expectWithinAbsoluteError<float>(processor.as_percent_of_max(1), 1.0f,
                                     0.01f,
                                     "incorrect output percentage for 1");

    beginTest("get_clipped_midi_volume");

    expect(processor.get_clipped_midi_volume(1.0f) == 127, "vol should 127");
    expect(processor.get_clipped_midi_volume(0.8f) == 102, "vol should 102");
    expect(processor.get_clipped_midi_volume(0.0f) == 0, "vol should 0");

    processor.set_volume_clip(50, 100);
    expect(processor.get_clipped_midi_volume(1.0f) == 100, "vol should 100");
    expect(processor.get_clipped_midi_volume(0.8f) == 90, "vol should 90");
    expect(processor.get_clipped_midi_volume(0.0f) == 0, "vol should 0");
    processor.set_volume_clip(1, 127);

    beginTest("get_note_velocity");

    expect(processor.get_note_velocity(0) == 0, "note vel should be 0");
    expect(processor.get_note_velocity(1) == 127, "note vel should be 127");

    processor.set_volume_clip(50, 80);
    expect(processor.get_note_velocity(0) == 0, "note vel should be 0");
    expect(processor.get_note_velocity(1) == 80, "note vel should be 80");

    processor.set_global_volume(0.5f);
    expect(processor.get_note_velocity(0) == 0, "note vel should be 0");
    expect(processor.get_note_velocity(1) == 40, "note vel should be 40");

    processor.set_global_volume(1.0f);
    processor.set_volume_clip(1, 127);

    // == render_buffer ==
    beginTest("render_buffer");

    processor.set_note_at(0, 60);
    processor.set_note_at(1, 64);
    processor.set_note_at(2, 67);

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
      expect(m.getVelocity() == 127, "note velocity is incorrect");
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
      expect(m.getVelocity() == 127, "note velocity is incorrect");
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
      expect(m.getVelocity() == 127, "note velocity is incorrect");
    }
  };
};

static MidiProcessorTests test;
