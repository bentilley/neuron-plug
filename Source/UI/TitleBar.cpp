/*
 * TitleBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "TitleBar.hpp"

TitleBar::TitleBar(WellsAudioProcessor &p)
    : processor(p), onOffButton("On/Off"), receivesMidiButton("MIDI In"),
      subdivisionSlider("Subdivision"), globalVolumeSlider("Volume"),
      volumeRange("MIDI Volume Range") {

  onOffButton.addListener(this);
  receivesMidiButton.addListener(this);

  subdivisionSlider.setSliderStyle(Slider::RotaryVerticalDrag);
  subdivisionSlider.setRange(1, 256, 1);
  subdivisionSlider.setTextBoxStyle(Slider::NoTextBox, false, 10, 0);
  subdivisionSlider.setPopupDisplayEnabled(true, false, this);

  globalVolumeSlider.setSliderStyle(Slider::RotaryVerticalDrag);
  globalVolumeSlider.setRange(0.0, 1.0, 0.00);
  globalVolumeSlider.setNumDecimalPlacesToDisplay(2);
  globalVolumeSlider.setTextBoxStyle(Slider::NoTextBox, false, 10, 0);
  globalVolumeSlider.setPopupDisplayEnabled(true, false, this);

  volumeRange.setSliderStyle(Slider::TwoValueHorizontal);
  volumeRange.setRange(0, 127, 1);
  volumeRange.setTextBoxStyle(Slider::NoTextBox, false, 10, 0);
  volumeRange.setPopupDisplayEnabled(true, false, this);

  addAndMakeVisible(onOffButton);
  addAndMakeVisible(receivesMidiButton);
  addAndMakeVisible(subdivisionSlider);
  addAndMakeVisible(globalVolumeSlider);
  addAndMakeVisible(volumeRange);
}
TitleBar::~TitleBar() {}

void TitleBar::paint(Graphics &g) {
  auto area = getLocalBounds();
  auto bottomBorder = area.removeFromBottom(bottomBorderPx);

  g.setColour(lightGrey);
  g.fillRect(area);
  g.setColour(darkGrey);
  g.fillRect(bottomBorder);

}

void TitleBar::resized() {

  auto area = getLocalBounds();
  area.removeFromBottom(bottomBorderPx);
  auto componentPadding = BorderSize<int>{10, 5, 10, 5};
  auto buttonArea = area.removeFromLeft(100);

  componentPadding.subtractFrom(buttonArea);
  onOffButton.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(100);
  componentPadding.subtractFrom(buttonArea);
  receivesMidiButton.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(100);
  componentPadding.subtractFrom(buttonArea);
  subdivisionSlider.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(100);
  componentPadding.subtractFrom(buttonArea);
  globalVolumeSlider.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(200);
  componentPadding.subtractFrom(buttonArea);
  volumeRange.setBounds(buttonArea);
}

void TitleBar::buttonClicked(Button *b) {
};
void TitleBar::buttonStateChanged(Button *b){};
