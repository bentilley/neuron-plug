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
      : FileLogger(File("~/Projects/fractal-flow/well-plug/plugin.log"),
                   String("Well Neuron Plugin Logs")){};
  ~PluginLogger(){};

  static PluginLogger logger;

  void log_vec(String vec_name, std::vector<int> vec);

private:
  bool isLogging{false};
};
