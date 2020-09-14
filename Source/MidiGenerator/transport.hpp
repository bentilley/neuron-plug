/*
 * transport.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

/** Container for DAW parameters.
 *
 * These are distinguished from PositionInfo which are the parameters associated
 * with the current buffer.
 */
struct SystemInfo {
  SystemInfo() = delete;
  SystemInfo(double sampleRate, int numBufferSamples)
      : sampleRate{sampleRate}, numBufferSamples{numBufferSamples}
  {
  }

  /** The current sample rate of the DAW, usually 44.1kHz. */
  double sampleRate;
  /** The current size of the audio buffer in samples, e.g. 512. */
  int numBufferSamples;
};
