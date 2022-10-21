/*
 * PluginLogger.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

class PluginLogger : public FileLogger {
public:
  PluginLogger()
      : FileLogger(
          File("~/Projects/fractal-flow/well-plug/plugin.log"),
          String("Well Neuron Plugin Logs")
        ){};
  ~PluginLogger(){};

  static PluginLogger logger;

  template <typename T> void logVec(String vecName, std::vector<T> vec)
  {
    if (!isLogging) {
      return;
    }
    String msg = vecName + ": (";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
      msg += String(*it);
      if (it != vec.end() - 1) {
        msg += ", ";
      }
    }
    msg += ")";
    logMessage(msg);
  }

  void logMidiMessage(String msgName, MidiMessage message);

private:
  bool isLogging{false};
};
