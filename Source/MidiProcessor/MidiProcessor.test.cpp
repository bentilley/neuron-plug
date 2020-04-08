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

    beginTest("Part 1");

    expect(processor.test(), "test is true");

    beginTest("Part 2");

    expect(processor.testFail(), "testFail is false");
  }
};

static MidiProcessorTests test;
