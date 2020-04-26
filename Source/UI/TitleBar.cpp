/*
 * TitleBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "TitleBar.hpp"

TitleBar::TitleBar(WellsAudioProcessor &p)
    : onOffButton(p), receivesMidiButton(p), subdivisionSlider(p),
      globalVolumeSlider(p), volumeRange(p) {

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
  subdivisionSlider.updateComponent();
  globalVolumeSlider.updateComponent();
  volumeRange.updateComponent();
}

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

/*
 * Subdivision Slider
 */

SubdivisionSlider::SubdivisionSlider(WellsAudioProcessor &p)
    : Slider("Subdivision"), processor(p) {
  setSliderStyle(Slider::RotaryVerticalDrag);
  setRange(1, 256, 1);
  setTextBoxStyle(Slider::NoTextBox, false, 10, 0);
  setPopupDisplayEnabled(true, false, getParentComponent());
  onValueChange = [this]() {
    processor.midiGenerator.set_subdivision(getValue());
  };
}
SubdivisionSlider::~SubdivisionSlider() {}

void SubdivisionSlider::updateComponent() {
  setValue(processor.midiGenerator.get_subdivision());
}

/*
 * Global Volume Slider
 */

GlobalVolumeSlider::GlobalVolumeSlider(WellsAudioProcessor &p)
    : Slider("Volume"), processor(p) {
  setSliderStyle(Slider::RotaryVerticalDrag);
  setRange(0.0, 1.0, 0.00);
  setNumDecimalPlacesToDisplay(2);
  setTextBoxStyle(Slider::NoTextBox, false, 10, 0);
  setPopupDisplayEnabled(true, false, getParentComponent());
  onValueChange = [this]() { processor.midiGenerator.set_volume(getValue()); };
}
GlobalVolumeSlider::~GlobalVolumeSlider() {}

void GlobalVolumeSlider::updateComponent() {
  setValue(processor.midiGenerator.get_volume());
}

/*
 * Volume Range Slider
 */

VolumeRangeSlider::VolumeRangeSlider(WellsAudioProcessor &p)
    : Slider("MIDI Volume Range"), processor(p) {
  setSliderStyle(Slider::TwoValueHorizontal);
  setRange(0, 127, 1);
  setTextBoxStyle(Slider::NoTextBox, false, 10, 0);
  setPopupDisplayEnabled(true, false, getParentComponent());
  onValueChange = [this]() {
    processor.midiGenerator.set_volume_clip(getMinValue(), getMaxValue());
  };
}
VolumeRangeSlider::~VolumeRangeSlider() {}

void VolumeRangeSlider::updateComponent() {
  setMinValue(processor.midiGenerator.get_volume_clip_min());
  setMaxValue(processor.midiGenerator.get_volume_clip_max());
}
