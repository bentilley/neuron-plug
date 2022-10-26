/*
 * TitleBar.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "../Utils/PluginLogger.hpp"
#include "Components/OnOffButton.hpp"
#include "Components/RotarySliderWithLabel.hpp"
#include "Styles.hpp"

/** Toggling the plugin on and off. */
class PluginOnOffButton : public OnOffButton {
public:
  PluginOnOffButton(WellsAudioProcessor& p);
  void updateComponent();
};

/** Changes the volume of all midi notes created. */
class GlobalVolumeSlider : public RotarySliderWithLabel {
public:
  GlobalVolumeSlider(WellsAudioProcessor& p);
  void updateComponent();
};

/** Changes the maximum note length of the MIDI output. */
class MaxNoteLengthSlider : public RotarySliderWithLabel {
public:
  MaxNoteLengthSlider(WellsAudioProcessor& p);
  void updateComponent();
};

/** Clips the volume range of midi notes created. */
class VolumeRangeSlider : public Slider {
public:
  VolumeRangeSlider(WellsAudioProcessor& p);
  void updateComponent();

private:
  WellsAudioProcessor& processor;
};

/** The main title bar at the top of the app. Holds the global controls:
 *   - On/Off Button
 *   - Beat Clock On/Off Button
 *   - MIDI Input On/Off Button
 *   - Subdivision Slider
 *   - Global Volume Slider
 *   - Volume Range Slider
 */
class TitleBar : public Component {
public:
  TitleBar(WellsAudioProcessor& p);

  void paint(Graphics&) override;
  void resized() override;

  void updateComponents();

private:
  int bottomBorderPx{1};
  BorderSize<int> componentPadding{10, 5, 10, 5};

  PluginOnOffButton onOffButton;
  GlobalVolumeSlider globalVolumeSlider;
  MaxNoteLengthSlider maxNoteLengthSlider;
  VolumeRangeSlider volumeRange;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitleBar)
};
