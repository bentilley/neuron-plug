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

    // == MIDI getters and setters ==
    beginTest("midi getters and setters");

    expect(generator.get_neuron_midi_note(0) == 60, "default should be 60");
    expect(generator.get_neuron_midi_note(1) == 64, "default should be 64");
    expect(generator.get_neuron_midi_note(2) == 67, "default should be 67");

    generator.set_neuron_midi_note(0, 34);
    generator.set_neuron_midi_note(1, 74);
    generator.set_neuron_midi_note(2, 100);

    expect(generator.get_neuron_midi_note(0) == 34, "midi note should be 34");
    expect(generator.get_neuron_midi_note(1) == 74, "midi note should be 74");
    expect(generator.get_neuron_midi_note(2) == 100, "midi note should be 100");

    // == Neuron input getters and setters ==
    beginTest("neuron input weights get/set");

    expect(generator.get_neuron_input_weight(0) == 0);
    expect(generator.get_neuron_input_weight(1) == 0);
    expect(generator.get_neuron_input_weight(2) == 0);

    generator.set_neuron_input_weight(0, 18);
    generator.set_neuron_input_weight(1, 5);
    generator.set_neuron_input_weight(2, 103);

    expect(generator.get_neuron_input_weight(0) == 18);
    expect(generator.get_neuron_input_weight(1) == 5);
    expect(generator.get_neuron_input_weight(2) == 103);

    // reset to default values for the rest of the tests
    generator.set_neuron_input_weight(0, 1);
    generator.set_neuron_input_weight(1, 1);
    generator.set_neuron_input_weight(2, 1);

    // == Neuron threshold getters and setters ==
    beginTest("neuron threshold get/set");

    expect(generator.get_neuron_threshold(0) == 0);
    expect(generator.get_neuron_threshold(1) == 0);
    expect(generator.get_neuron_threshold(2) == 0);

    generator.set_neuron_threshold(0, 8);
    generator.set_neuron_threshold(1, 52);
    generator.set_neuron_threshold(2, 10);

    expect(generator.get_neuron_threshold(0) == 8);
    expect(generator.get_neuron_threshold(1) == 52);
    expect(generator.get_neuron_threshold(2) == 10);

    // reset to default values for the rest of the tests
    generator.set_neuron_threshold(0, 0);
    generator.set_neuron_threshold(1, 0);
    generator.set_neuron_threshold(2, 0);

    // == Neuron connection weights getters and setters ==
    beginTest("neuron connection weights get/set");

    expect(generator.get_neuron_connection_weight(0, 0) == 0);
    expect(generator.get_neuron_connection_weight(0, 1) == 0);
    expect(generator.get_neuron_connection_weight(0, 2) == 0);
    expect(generator.get_neuron_connection_weight(1, 0) == 0);
    expect(generator.get_neuron_connection_weight(1, 1) == 0);
    expect(generator.get_neuron_connection_weight(1, 2) == 0);
    expect(generator.get_neuron_connection_weight(2, 0) == 0);
    expect(generator.get_neuron_connection_weight(2, 1) == 0);
    expect(generator.get_neuron_connection_weight(2, 2) == 0);

    generator.set_neuron_connection_weight(0, 0, 18);
    generator.set_neuron_connection_weight(0, 1, -3);
    generator.set_neuron_connection_weight(0, 2, 8);
    generator.set_neuron_connection_weight(1, 0, 13);
    generator.set_neuron_connection_weight(1, 1, 38);
    generator.set_neuron_connection_weight(1, 2, -8);
    generator.set_neuron_connection_weight(2, 0, -58);
    generator.set_neuron_connection_weight(2, 1, 1);
    generator.set_neuron_connection_weight(2, 2, 180);

    expect(generator.get_neuron_connection_weight(0, 0) == 18);
    expect(generator.get_neuron_connection_weight(0, 1) == -3);
    expect(generator.get_neuron_connection_weight(0, 2) == 8);
    expect(generator.get_neuron_connection_weight(1, 0) == 13);
    expect(generator.get_neuron_connection_weight(1, 1) == 38);
    expect(generator.get_neuron_connection_weight(1, 2) == -8);
    expect(generator.get_neuron_connection_weight(2, 0) == -58);
    expect(generator.get_neuron_connection_weight(2, 1) == 1);
    expect(generator.get_neuron_connection_weight(2, 2) == 180);

    // reset to default values for the rest of the tests
    generator.set_neuron_connection_weight(0, 0, 0);
    generator.set_neuron_connection_weight(0, 1, 0);
    generator.set_neuron_connection_weight(0, 2, 0);
    generator.set_neuron_connection_weight(1, 0, 0);
    generator.set_neuron_connection_weight(1, 1, 0);
    generator.set_neuron_connection_weight(1, 2, 0);
    generator.set_neuron_connection_weight(2, 0, 0);
    generator.set_neuron_connection_weight(2, 1, 0);
    generator.set_neuron_connection_weight(2, 2, 0);

    // == Neuron Model Methods ==
    beginTest("Neuron Model Methods");

    expect(generator.num_neurons() == 3, "model should load with 3 neurons");

    generator.add_neuron();
    generator.add_neuron();

    expect(generator.num_neurons() == 5, "model should now have 5 neurons");

    expect(generator.get_neuron_connection_weight(3, 0) == 0);
    expect(generator.get_neuron_connection_weight(3, 1) == 0);
    expect(generator.get_neuron_connection_weight(3, 2) == 0);
    expect(generator.get_neuron_connection_weight(3, 3) == 0);
    expect(generator.get_neuron_connection_weight(3, 4) == 0);

    expect(generator.get_neuron_connection_weight(4, 0) == 0);
    expect(generator.get_neuron_connection_weight(4, 1) == 0);
    expect(generator.get_neuron_connection_weight(4, 2) == 0);
    expect(generator.get_neuron_connection_weight(4, 3) == 0);
    expect(generator.get_neuron_connection_weight(4, 4) == 0);

    expect(generator.get_neuron_connection_weight(0, 3) == 0);
    expect(generator.get_neuron_connection_weight(0, 4) == 0);
    expect(generator.get_neuron_connection_weight(1, 3) == 0);
    expect(generator.get_neuron_connection_weight(1, 4) == 0);
    expect(generator.get_neuron_connection_weight(2, 3) == 0);
    expect(generator.get_neuron_connection_weight(2, 4) == 0);

    generator.remove_neuron();
    generator.remove_neuron();

    expect(generator.num_neurons() == 3, "model should now have 3 neurons");

    generator.set_neuron_connection_weight(0, 0, 1);
    generator.set_neuron_connection_weight(0, 1, 2);
    generator.set_neuron_connection_weight(0, 2, 3);
    generator.set_neuron_connection_weight(1, 0, 4);
    generator.set_neuron_connection_weight(1, 1, 5);
    generator.set_neuron_connection_weight(1, 2, 6);
    generator.set_neuron_connection_weight(2, 0, 7);
    generator.set_neuron_connection_weight(2, 1, 8);
    generator.set_neuron_connection_weight(2, 2, 9);

    generator.remove_neuron_at(0);

    expect(generator.get_neuron_connection_weight(0, 0) == 5);
    expect(generator.get_neuron_connection_weight(0, 1) == 6);
    expect(generator.get_neuron_connection_weight(1, 0) == 8);
    expect(generator.get_neuron_connection_weight(1, 1) == 9);

    generator.remove_neuron_at(1);

    expect(generator.get_neuron_connection_weight(0, 0) == 5);

    generator.add_neuron();
    generator.add_neuron();
    generator.set_neuron_connection_weight(0, 0, 0);

    // == generate_next_midi_buffer ==
    beginTest("generate_next_midi_buffer");

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
