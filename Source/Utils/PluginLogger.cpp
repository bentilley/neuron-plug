/*
 * PluginLogger.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "PluginLogger.hpp"

void PluginLogger::log_vec(String vec_name, std::vector<int> vec) {
  String msg = vec_name + ": (";
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    msg += String(*it);
    if (it != vec.end() - 1) {
      msg += ", ";
    }
  }
  msg += ")";
  logMessage(msg);
}

PluginLogger PluginLogger::logger{};
