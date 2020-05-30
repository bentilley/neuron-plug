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

// On/Off Button - toggling the plugin on and off.

class OnOffButton : public TextButton {
public:
  OnOffButton(WellsAudioProcessor &p);
  ~OnOffButton();
  void updateComponent();

private:
  WellsAudioProcessor &processor;
};

// Receives MIDI Button - for toggling whether the plugin responds to MIDI in.

class ReceivesMidiButton : public TextButton {
public:
  ReceivesMidiButton(WellsAudioProcessor &p);
  ~ReceivesMidiButton();
  void updateComponent();

private:
  WellsAudioProcessor &processor;
};

/*
 * Subdivision Slider - changes the subdivision in the BeatClock
 */

class SubdivisionSlider : public Slider {
public:
  SubdivisionSlider(WellsAudioProcessor &p);
  ~SubdivisionSlider();
  void updateComponent();

private:
  WellsAudioProcessor &processor;
};

/*
 * Global Volume Slider - changes the volume of all midi notes created
 */

class GlobalVolumeSlider : public Slider {
public:
  GlobalVolumeSlider(WellsAudioProcessor &p);
  ~GlobalVolumeSlider();
  void updateComponent();

private:
  WellsAudioProcessor &processor;
};

/*
 * Volume Range Slider - clips the volume range of midi notes created
 */

class VolumeRangeSlider : public Slider {
public:
  VolumeRangeSlider(WellsAudioProcessor &p);
  ~VolumeRangeSlider();
  void updateComponent();

private:
  WellsAudioProcessor &processor;
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

class TitleBar : public Component {
public:
  TitleBar(WellsAudioProcessor &p);
  ~TitleBar();

  void paint(Graphics &) override;
  void resized() override;

  void updateComponents();

private:
  int bottomBorderPx{1};
  BorderSize<int> componentPadding{10, 5, 10, 5};
  int componentWidth{100};

  OnOffButton onOffButton;
  ReceivesMidiButton receivesMidiButton;
  SubdivisionSlider subdivisionSlider;
  GlobalVolumeSlider globalVolumeSlider;
  VolumeRangeSlider volumeRange;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitleBar)
};
