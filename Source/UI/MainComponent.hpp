/*
 * MainComponent.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginBody.hpp"
#include "TitleBar.hpp"

class MainComponent : public Component {
public:
  MainComponent();
  ~MainComponent();

  void paint(Graphics &) override;
  void resized() override;

private:
  String currentSizeAsString;

  int titleHeight{80};

  TitleBar titleBar;
  PluginBody pluginBody;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
