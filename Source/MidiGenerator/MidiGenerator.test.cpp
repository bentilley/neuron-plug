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

    expect(generator.test());
  };
};

static MidiGeneratorTests test;
