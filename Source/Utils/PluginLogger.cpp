/*
 * PluginLogger.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "PluginLogger.hpp"

void PluginLogger::logMidiMessage(String msgName, MidiMessage message)
{
  if (!isLogging) {
    return;
  }

  String msg = msgName + ": (";
  msg += message.getDescription();
  msg += ")";
  logMessage(msg);
}

PluginLogger PluginLogger::logger{};
