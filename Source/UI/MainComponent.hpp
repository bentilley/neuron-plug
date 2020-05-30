/*
 * MainComponent.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "PluginBody.hpp"
#include "TitleBar.hpp"

class MainComponent : public Component, private Timer {
public:
  MainComponent(WellsAudioProcessor &p);
  ~MainComponent();

  void paint(Graphics &) override;
  void resized() override;

private:
  WellsAudioProcessor &processor;

  TitleBar titleBar;
  PluginBody pluginBody;

  void timerCallback() override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
