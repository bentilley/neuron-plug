/*
 * TitleBar.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "../Utils/PluginLogger.hpp"

// On/Off Button - toggling the plugin on and off.

class OnOffButton : public TextButton {
public:
  OnOffButton(WellsAudioProcessor &p);
  ~OnOffButton();
  void updateComponent();

private:
  WellsAudioProcessor &processor;
  Colour buttonOnColour{133, 150, 241};
  Colour buttonOffColour{39, 44, 71};
};

// Receives MIDI Button - for toggling whether the plugin responds to MIDI in.

class ReceivesMidiButton : public TextButton {
public:
  ReceivesMidiButton(WellsAudioProcessor &p);
  ~ReceivesMidiButton();
  void updateComponent();

private:
  WellsAudioProcessor &processor;
  Colour buttonOnColour{133, 150, 241};
  Colour buttonOffColour{39, 44, 71};
};

/*
 * Title Bar
 *
 * The main title bar at the top of the app. Holds the global controls:
 *   - On/Off Button
 *   - Receives MIDI Button
 *   - Subdivision Slider
 *   - Global Volume Slider
 *   - Volume Range Slider
 */

class TitleBar : public Component, private Slider::Listener {
public:
  TitleBar(WellsAudioProcessor &p);
  ~TitleBar();

  void paint(Graphics &) override;
  void resized() override;

  void updateComponents();

private:
  WellsAudioProcessor &processor;

  Colour lightGrey{221, 221, 221};
  Colour darkGrey{51, 51, 51};

  int bottomBorderPx{1};
  BorderSize<int> componentPadding{10, 5, 10, 5};
  int componentWidth{100};

  OnOffButton onOffButton;
  ReceivesMidiButton receivesMidiButton;
  Slider subdivisionSlider;
  Slider globalVolumeSlider;
  Slider volumeRange;

  void sliderValueChanged(Slider *slider) override;
  void sliderDragStarted(Slider *slider) override;
  void sliderDragEnded(Slider *slider) override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitleBar)
};
