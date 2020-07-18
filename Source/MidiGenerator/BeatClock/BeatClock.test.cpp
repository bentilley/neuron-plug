/*
 * BeatClock.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <catch2/catch.hpp>

#define private public
#include "BeatClock.hpp"

SCENARIO("BeatClock - getters and setters") {
  GIVEN("an instance of a BeatClock") {
    BeatClock clock;

    REQUIRE(clock.getSubdivision() == 1);
    REQUIRE(clock.getOutputScaleFactor() == Approx(1.0f));

    WHEN("we set the subdivision") {
      clock.setSubdivision(12);
      REQUIRE(clock.getSubdivision() == 12);

      clock.setSubdivision(79);
      REQUIRE(clock.getSubdivision() == 79);

      clock.setSubdivision(2);
      REQUIRE(clock.getSubdivision() == 2);
    }

    WHEN("we set the output scale factor") {
      clock.setOutputScaleFactor(0.286);
      REQUIRE(clock.getOutputScaleFactor() == Approx(0.286));

      clock.setOutputScaleFactor(2.551);
      REQUIRE(clock.getOutputScaleFactor() == Approx(2.551));

      clock.setOutputScaleFactor(1.00035);
      REQUIRE(clock.getOutputScaleFactor() == Approx(1.00035));
    }
  }
}

SCENARIO("BeatClock - no play-tick in the current buffer") {
  GIVEN("an instance of a BeatClock") {
    BeatClock clock;

    GIVEN("playhead position info and system info") {
      AudioPlayHead::CurrentPositionInfo pos;
      double sampleRate = 44100;
      int numBufferSamples = 256;
      SystemInfo sys{sampleRate, numBufferSamples};

      WHEN("BPM: 120, timeInSamples: 21760") {
        pos.bpm = 120;
        pos.timeInSamples = 21760;

        REQUIRE(clock.getModelInputForBuffer(pos, sys).size() == 0);
      }

      WHEN("BPM: 120, timeInSamples: 10752, subdivision: 2") {
        pos.bpm = 120;
        pos.timeInSamples = 10752;
        clock.setSubdivision(2);

        REQUIRE(clock.getModelInputForBuffer(pos, sys).size() == 0);
      }

      WHEN("BPM: 114, timeInSamples: 4096, subdivision: 4, buffSize: 1024") {
        pos.bpm = 114;
        pos.timeInSamples = 4096;
        sys.numBufferSamples = 512;
        clock.setSubdivision(4);

        REQUIRE(clock.getModelInputForBuffer(pos, sys).size() == 0);
      }
    }
  }
}

SCENARIO("BeatClock - has play-tick in the current buffer") {
  GIVEN("an instance of a BeatClock") {
    BeatClock clock;

    GIVEN("playhead position info and system info") {
      AudioPlayHead::CurrentPositionInfo pos;
      double sampleRate = 44100;
      int numBufferSamples = 256;
      SystemInfo sys{sampleRate, numBufferSamples};

      WHEN("BPM: 100, timeInSamples: 13056, subdivision: 2") {
        pos.bpm = 100;
        pos.timeInSamples = 13056;
        clock.setSubdivision(2);

        auto result = clock.getModelInputForBuffer(pos, sys);
        REQUIRE(result.size() == 1);
      }

      WHEN("BPM: 120, timeInSamples: 2560, subdivision: 8, buffSize: 512") {
        pos.bpm = 120;
        pos.timeInSamples = 2560;
        sys.numBufferSamples = 512;
        clock.setSubdivision(8);

        auto result = clock.getModelInputForBuffer(pos, sys);
        REQUIRE(result.size() == 1);
      }

      WHEN("BPM: 60, timeInSamples: 44032, subdivision: 1") {
        pos.bpm = 60;
        pos.timeInSamples = 44032;
        clock.setSubdivision(1);

        auto result = clock.getModelInputForBuffer(pos, sys);
        REQUIRE(result.size() == 1);
      }
    }
  }
}

SCENARIO("BeatClock - with multiple play-ticks in the current buffer") {
  GIVEN("an instance of a BeatClock") {
    BeatClock clock;

    GIVEN("playhead position info and system info") {
      AudioPlayHead::CurrentPositionInfo pos;
      double sampleRate = 44100;
      int numBufferSamples = 256;
      SystemInfo sys{sampleRate, numBufferSamples};

      WHEN("BPM: 120, timeInSamples: 6144, subdivision: 25, buffSize: 1024") {
        pos.bpm = 120;
        pos.timeInSamples = 6144;
        sys.numBufferSamples = 1024;
        clock.setSubdivision(25);

        REQUIRE(clock.getModelInputForBuffer(pos, sys).size() == 2);
      }

      WHEN("BPM: 99, timeInSamples: 1024, subdivision: 50, buffSize: 1024") {
        pos.bpm = 99;
        pos.timeInSamples = 1024;
        sys.numBufferSamples = 1024;
        clock.setSubdivision(50);

        REQUIRE(clock.getModelInputForBuffer(pos, sys).size() == 2);
      }

      WHEN("BPM: 166, timeInSamples: 3072, subdivision: 20, buffSize: 1024") {
        pos.bpm = 166;
        pos.timeInSamples = 3072;
        sys.numBufferSamples = 1024;
        clock.setSubdivision(20);

        REQUIRE(clock.getModelInputForBuffer(pos, sys).size() == 2);
      }
    }
  }
}

SCENARIO("BeatClock - changing the output scale factor") {
  GIVEN("an instance of a BeatClock") {
    BeatClock clock;

    REQUIRE(clock.getSubdivision() == 1);
    REQUIRE(clock.getOutputScaleFactor() == Approx(1.0f));

    GIVEN("playhead position info and system info") {
      AudioPlayHead::CurrentPositionInfo pos;
      double sampleRate = 44100;
      int numBufferSamples = 256;
      SystemInfo sys{sampleRate, numBufferSamples};

      WHEN("BPM: 100, timeInSamples: 13056, subdivision: 2") {
        pos.bpm = 100;
        pos.timeInSamples = 13056;
        clock.setSubdivision(2);

        THEN("the output scale factor is changed to 0.5") {
          clock.setOutputScaleFactor(0.5);

          auto result = clock.getModelInputForBuffer(pos, sys);
          REQUIRE(result.size() == 1);
          for (auto value : result.at(0).data) {
            REQUIRE(value == Approx(0.5f));
          }
        }

        THEN("the output scale factor is changed to 1.453") {
          clock.setOutputScaleFactor(1.453);

          auto result = clock.getModelInputForBuffer(pos, sys);
          REQUIRE(result.size() == 1);
          for (auto value : result.at(0).data) {
            REQUIRE(value == Approx(1.453f));
          }
        }
      }
    }
  }
}

SCENARIO("BeatClock - private methods") {
  GIVEN("an instance of a BeatClock") {
    BeatClock clock;

    GIVEN("BPM and sample rate") {
      float bpm;
      double sampleRate = 44100.0;

      WHEN("BPM: 120, subdivision: 1") {
        bpm = 120.0f;
        REQUIRE(clock.getSamplesPerSubdivision(bpm, sampleRate) ==
                Approx(22050.0f));
      }

      WHEN("BPM: 114, subdivision: 4") {
        bpm = 114.0f;
        clock.setSubdivision(4);
        REQUIRE(clock.getSamplesPerSubdivision(bpm, sampleRate) ==
                Approx(5802.631579f));
      }

      WHEN("BPM: 99, subdivision: 50") {
        bpm = 99.0f;
        clock.setSubdivision(50);
        REQUIRE(clock.getSamplesPerSubdivision(bpm, sampleRate) ==
                Approx(534.545454f));
      }
    }

    GIVEN("BPM, sample rate, and the time in samples") {
      float bpm;
      int64_t timeInSamples;
      double sampleRate = 44100.0;

      WHEN("BPM: 120, subdivision: 1, timeInSamples: 2459723749") {
        bpm = 120.0f;
        timeInSamples = 2459723749;
        REQUIRE(clock.getNumberOfNextHit(bpm, timeInSamples, sampleRate) ==
                111553);
      }

      WHEN("BPM: 114, subdivision: 4, timeInSamples: 937502") {
        bpm = 114.0f;
        timeInSamples = 937502;
        clock.setSubdivision(4);
        REQUIRE(clock.getNumberOfNextHit(bpm, timeInSamples, sampleRate) ==
                162);
      }

      WHEN("BPM: 99, subdivision: 50, timeInSamples: 448375") {
        bpm = 99.0f;
        timeInSamples = 448375;
        clock.setSubdivision(50);
        REQUIRE(clock.getNumberOfNextHit(bpm, timeInSamples, sampleRate) ==
                839);
      }
    }
  }
}
