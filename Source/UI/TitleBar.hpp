/*
 * TitleBar.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

class TitleBar : public Component, private Button::Listener {
public:
  TitleBar(WellsAudioProcessor &p);
  ~TitleBar();

  void paint(Graphics &) override;
  void resized() override;

private:
  WellsAudioProcessor &processor;

  Colour lightGrey{221, 221, 221};
  Colour darkGrey{51, 51, 51};
  Colour buttonOnColour{255, 0, 0};
  Colour buttonOffColour{0, 255, 0};

  int bottomBorderPx{1};
  BorderSize<int> componentPadding{10, 5, 10, 5};
  int componentWidth{100};

  TextButton onOffButton;
  TextButton receivesMidiButton;
  Slider subdivisionSlider;
  Slider globalVolumeSlider;
  Slider volumeRange;

  // methods

  void buttonClicked(Button *b) override;
  void buttonStateChanged(Button *b) override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitleBar)
};
