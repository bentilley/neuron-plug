/*
 * BeatClock.test.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "BeatClock.hpp"
#include <catch2/catch.hpp>

SCENARIO("BeatClock") {
  GIVEN("an instance of a BeatClock") {
    BeatClock clock;

    REQUIRE(!clock.is_configured());
    REQUIRE(clock.get_subdivision() == 1);

    WHEN("we configure the BeatClock") {
      double sample_rate = 44100;
      AudioPlayHead::CurrentPositionInfo pos;
      pos.bpm = 120;
      pos.timeInSamples = 905984;
      clock.configure(sample_rate, pos);

      REQUIRE(clock.is_configured());

      THEN("reset the clock") {
        clock.reset();

        REQUIRE(!clock.is_configured());
      }
    }
  }
}

SCENARIO("BeatClock Configuration") {
  GIVEN("an instance of BeatClock") {
    BeatClock clock;
    double sample_rate = 44100;
    AudioPlayHead::CurrentPositionInfo pos;

    WHEN("BPM: 120, timeInSamples: 905984") {
      pos.bpm = 120;
      pos.timeInSamples = 905984;
      clock.configure(sample_rate, pos);

      REQUIRE(clock.get_sample_num_remainder() == Approx(1934.0));
      REQUIRE(clock.get_samples_per_subdivision() == Approx(22050.0));
    }

    WHEN("BPM: 100, timeInSamples: 4905984") {
      pos.bpm = 100;
      pos.timeInSamples = 4905984;
      clock.configure(sample_rate, pos);

      REQUIRE(clock.get_sample_num_remainder() == Approx(10884.0));
      REQUIRE(clock.get_samples_per_subdivision() == Approx(26460.0));
    }

    WHEN("BPM: 149, timeInSamples: 1706784") {
      pos.bpm = 149;
      pos.timeInSamples = 1706784;
      clock.configure(sample_rate, pos);

      REQUIRE(clock.get_sample_num_remainder() == Approx(1978.69));
      REQUIRE(clock.get_samples_per_subdivision() == Approx(17758.39));
    }

    GIVEN("subdivision is set to 2") {
      clock.set_subdivision(2);

      WHEN("BPM: 69, timeInSamples: 642684") {
        pos.bpm = 69;
        pos.timeInSamples = 642684;
        clock.configure(sample_rate, pos);

        REQUIRE(clock.get_sample_num_remainder() == Approx(9944.90));
        REQUIRE(clock.get_samples_per_subdivision() == Approx(19173.91));
      }
    }

    GIVEN("subdivision is set to 5") {
      clock.set_subdivision(5);

      WHEN("BPM: 111, timeInSamples: 34701294") {
        pos.bpm = 111;
        pos.timeInSamples = 34701294;
        clock.configure(sample_rate, pos);

        REQUIRE(clock.get_sample_num_remainder() == Approx(2938.59));
        REQUIRE(clock.get_samples_per_subdivision() == Approx(4767.57));
      }
    }
  }
}

SCENARIO("BeatClock Should Play") {
  GIVEN("an instance of BeatClock") {
    BeatClock clock;
    double sample_rate = 44100;
    AudioPlayHead::CurrentPositionInfo pos;

    WHEN("BPM: 120, timeInSamples: 905984") {
      pos.bpm = 120;
      pos.timeInSamples = 905984;
      clock.configure(sample_rate, pos);

      REQUIRE(!clock.should_play(20114));
      REQUIRE(!clock.should_play(20115));
      REQUIRE(clock.should_play(20116));
      REQUIRE(!clock.should_play(20117));
      REQUIRE(!clock.should_play(20118));
    }

    WHEN("BPM: 100, timeInSamples: 4905984") {
      pos.bpm = 100;
      pos.timeInSamples = 4905984;
      clock.configure(sample_rate, pos);

      REQUIRE(!clock.should_play(15574));
      REQUIRE(!clock.should_play(15575));
      REQUIRE(clock.should_play(15576));
      REQUIRE(!clock.should_play(15577));
      REQUIRE(!clock.should_play(15578));
    }

    WHEN("BPM: 149, timeInSamples: 1706784") {
      pos.bpm = 149;
      pos.timeInSamples = 1706784;
      clock.configure(sample_rate, pos);

      REQUIRE(!clock.should_play(15778));
      REQUIRE(!clock.should_play(15779));
      REQUIRE(clock.should_play(15780));
      REQUIRE(!clock.should_play(15781));
      REQUIRE(!clock.should_play(15782));
    }

    GIVEN("subdivision is set to 2") {
      clock.set_subdivision(2);

      WHEN("BPM: 69, timeInSamples: 642684") {
        pos.bpm = 69;
        pos.timeInSamples = 642684;
        clock.configure(sample_rate, pos);

        REQUIRE(!clock.should_play(9228));
        REQUIRE(!clock.should_play(9229));
        REQUIRE(clock.should_play(9230));
        REQUIRE(!clock.should_play(9231));
        REQUIRE(!clock.should_play(9232));
      }
    }

    GIVEN("subdivision is set to 5") {
      clock.set_subdivision(5);

      WHEN("BPM: 111, timeInSamples: 34701294") {
        pos.bpm = 111;
        pos.timeInSamples = 34701294;
        clock.configure(sample_rate, pos);

        REQUIRE(!clock.should_play(1827));
        REQUIRE(!clock.should_play(1828));
        REQUIRE(clock.should_play(1829));
        REQUIRE(!clock.should_play(1830));
        REQUIRE(!clock.should_play(1831));
      }
    }
  }
}
