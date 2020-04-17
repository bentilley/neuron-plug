/*
 * PluginLogger.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "PluginLogger.hpp"

void PluginLogger::log_vec(String vec_name, std::vector<int> vec) {
  logMessage(vec_name + ": (" + String(vec.at(0)) + ", " + String(vec.at(1)) +
             ", " + String(vec.at(2)) + ")");
}

PluginLogger PluginLogger::logger{};
