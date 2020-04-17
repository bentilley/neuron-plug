/*
 * TitleBar.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

class TitleBar : public Component {
public:
  TitleBar(WellsAudioProcessor &p);
  ~TitleBar();

  void paint(Graphics &) override;
  void resized() override;

private:
  WellsAudioProcessor &processor;

  Colour lightGrey{221, 221, 221};
  Colour darkGrey{51, 51, 51};

  int bottomBorderPx{1};

  TextButton onOffButton;
  TextButton receivesMidiButton;
  Slider subdivisionSlider;
  Slider globalVolumeSlider;
  Slider volumeRange;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitleBar)
};
