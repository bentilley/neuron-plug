/*
 * MainComponent.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MainComponent : public Component, private Slider::Listener {
public:
  MainComponent();
  ~MainComponent();

  void paint(Graphics &) override;
  void resized() override;

private:
  String currentSizeAsString;
  Slider midiVolume;

  void sliderValueChanged(Slider *slider) override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
