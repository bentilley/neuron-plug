/*
 * TitleBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "TitleBar.hpp"

TitleBar::TitleBar(WellsAudioProcessor &p)
    : processor(p), onOffButton(p), receivesMidiButton(p),
      subdivisionSlider("Subdivision"), globalVolumeSlider("Volume"),
      volumeRange("MIDI Volume Range") {

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
  auto buttonArea = area.removeFromLeft(componentWidth);

  componentPadding.subtractFrom(buttonArea);
  onOffButton.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(componentWidth);
  componentPadding.subtractFrom(buttonArea);
  receivesMidiButton.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(componentWidth);
  componentPadding.subtractFrom(buttonArea);
  subdivisionSlider.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(componentWidth);
  componentPadding.subtractFrom(buttonArea);
  globalVolumeSlider.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(2 * componentWidth);
  componentPadding.subtractFrom(buttonArea);
  volumeRange.setBounds(buttonArea);
}

void TitleBar::updateComponents() {
  // TODO only update GUI when needed
  onOffButton.updateComponent();
  receivesMidiButton.updateComponent();
}

void TitleBar::sliderValueChanged(Slider *s) {
  if (s == &subdivisionSlider) {

  } else if (s == &globalVolumeSlider) {

  } else if (s == &volumeRange) {
  }
}
void TitleBar::sliderDragStarted(Slider *s) {}
void TitleBar::sliderDragEnded(Slider *s) {}

/*
 * On/Off Button
 */

OnOffButton::OnOffButton(WellsAudioProcessor &p)
    : TextButton("On/Off"), processor(p) {
  onClick = [this]() { processor.midiGenerator.toggleOnOff(); };
}
OnOffButton::~OnOffButton() {}

void OnOffButton::updateComponent() {
  setColour(TextButton::ColourIds::buttonColourId,
            processor.midiGenerator.get_is_on() ? buttonOnColour
                                                : buttonOffColour);
}

/*
 * Receives MIDI Button
 */

ReceivesMidiButton::ReceivesMidiButton(WellsAudioProcessor &p)
    : TextButton("MIDI In"), processor(p) {
  onClick = [this]() { processor.midiGenerator.toggleReceivesMidi(); };
}
ReceivesMidiButton::~ReceivesMidiButton() {}

void ReceivesMidiButton::updateComponent() {
  setColour(TextButton::ColourIds::buttonColourId,
            processor.midiGenerator.get_receives_midi() ? buttonOnColour
                                                        : buttonOffColour);
}
