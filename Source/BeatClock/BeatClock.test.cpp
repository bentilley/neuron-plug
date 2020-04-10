/*
 * BeatClock.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "BeatClock.hpp"

class BeatClockTests : public UnitTest {
public:
  BeatClockTests() : UnitTest("BeatClock Testing") {}

  void runTest() override {
    BeatClock clock;

    // == Instantiation ==
    beginTest("Instantiation");

    expect(!clock.is_configured(), "Clock is prematurely configured");
    expect(clock.get_subdivision() == 1, "Clock has wrong default subdivision");

    // == configure ==
    beginTest("configure");

    double sample_rate = 44100;
    AudioPlayHead::CurrentPositionInfo pos;

    pos.bpm = 120;
    pos.timeInSamples = 905984;
    clock.configure(sample_rate, pos);
    expectWithinAbsoluteError<float>(clock.get_sample_num_remainder(), 1934.0,
                                     0.01,
                                     "current_sample_remainder is not correct");
    expectWithinAbsoluteError<float>(clock.get_samples_per_subdivision(),
                                     22050.0, 0.01,
                                     "samples_per_subdivision is not correct");

    pos.bpm = 100;
    pos.timeInSamples = 4905984;
    clock.configure(sample_rate, pos);
    expectWithinAbsoluteError<float>(clock.get_sample_num_remainder(), 10884.0,
                                     0.01,
                                     "current_sample_remainder is not correct");
    expectWithinAbsoluteError<float>(clock.get_samples_per_subdivision(),
                                     26460.0, 0.01,
                                     "samples_per_subdivision is not correct");

    pos.bpm = 149;
    pos.timeInSamples = 1706784;
    clock.configure(sample_rate, pos);
    expectWithinAbsoluteError<float>(clock.get_sample_num_remainder(), 1978.69,
                                     0.01,
                                     "current_sample_remainder is not correct");
    expectWithinAbsoluteError<float>(clock.get_samples_per_subdivision(),
                                     17758.39, 0.01,
                                     "samples_per_subdivision is not correct");

    clock.set_subdivision(2);
    pos.bpm = 69;
    pos.timeInSamples = 642684;
    clock.configure(sample_rate, pos);
    expectWithinAbsoluteError<float>(clock.get_sample_num_remainder(), 9944.90,
                                     0.01,
                                     "current_sample_remainder is not correct");
    expectWithinAbsoluteError<float>(clock.get_samples_per_subdivision(),
                                     19173.91, 0.01,
                                     "samples_per_subdivision is not correct");

    clock.set_subdivision(5);
    pos.bpm = 111;
    pos.timeInSamples = 34701294;
    clock.configure(sample_rate, pos);
    expectWithinAbsoluteError<float>(clock.get_sample_num_remainder(), 2938.59,
                                     0.01,
                                     "current_sample_remainder is not correct");
    expectWithinAbsoluteError<float>(clock.get_samples_per_subdivision(),
                                     4767.57, 0.01,
                                     "samples_per_subdivision is not correct");

    // == should_play ==
    beginTest("should_play");

    pos.bpm = 120;
    pos.timeInSamples = 905984;
    clock.configure(sample_rate, pos);
    expect(!clock.should_play(20114), "clock shouldn't play on sample 20114");
    expect(!clock.should_play(20115), "clock shouldn't play on sample 20115");
    expect(clock.should_play(20116), "clock should play on sample 20116");
    expect(!clock.should_play(20117), "clock shouldn't play on sample 20117");
    expect(!clock.should_play(20118), "clock shouldn't play on sample 20118");

    pos.bpm = 100;
    pos.timeInSamples = 4905984;
    clock.configure(sample_rate, pos);
    expect(!clock.should_play(15574), "clock shouldn't play on sample 15574");
    expect(!clock.should_play(15575), "clock shouldn't play on sample 15575");
    expect(clock.should_play(15576), "clock should play on sample 15576");
    expect(!clock.should_play(15577), "clock shouldn't play on sample 15577");
    expect(!clock.should_play(15578), "clock shouldn't play on sample 15578");

    pos.bpm = 149;
    pos.timeInSamples = 1706784;
    clock.configure(sample_rate, pos);
    expect(!clock.should_play(15778), "clock shouldn't play on sample 15778");
    expect(!clock.should_play(15779), "clock shouldn't play on sample 15779");
    expect(clock.should_play(15780), "clock should play on sample 15780");
    expect(!clock.should_play(15781), "clock shouldn't play on sample 15781");
    expect(!clock.should_play(15782), "clock shouldn't play on sample 15782");

    clock.set_subdivision(2);
    pos.bpm = 69;
    pos.timeInSamples = 642684;
    clock.configure(sample_rate, pos);
    expect(!clock.should_play(9228), "clock shouldn't play on sample 9228");
    expect(!clock.should_play(9229), "clock shouldn't play on sample 9229");
    expect(clock.should_play(9230), "clock should play on sample 9230");
    expect(!clock.should_play(9231), "clock shouldn't play on sample 9231");
    expect(!clock.should_play(9232), "clock shouldn't play on sample 9232");

    clock.set_subdivision(5);
    pos.bpm = 111;
    pos.timeInSamples = 34701294;
    clock.configure(sample_rate, pos);
    expect(!clock.should_play(1827), "clock shouldn't play on sample 1827");
    expect(!clock.should_play(1828), "clock shouldn't play on sample 1828");
    expect(clock.should_play(1829), "clock should play on sample 1829");
    expect(!clock.should_play(1830), "clock shouldn't play on sample 1830");
    expect(!clock.should_play(1831), "clock shouldn't play on sample 1831");

    // == reset ==
    beginTest("reset");
    clock.reset();
    expect(!clock.is_configured());
  };
};

static BeatClockTests test;
