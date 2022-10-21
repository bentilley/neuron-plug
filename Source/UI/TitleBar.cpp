/*
 * TitleBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "TitleBar.hpp"
#include "Styles.hpp"

TitleBar::TitleBar(WellsAudioProcessor& p)
    : onOffButton(p), beatClockOnOffButton(p), midiInputOnOffButton(p), subdivisionSlider(p),
      globalVolumeSlider(p), volumeRange(p)
{

  addAndMakeVisible(onOffButton);
  addAndMakeVisible(beatClockOnOffButton);
  addAndMakeVisible(midiInputOnOffButton);
  addAndMakeVisible(subdivisionSlider);
  addAndMakeVisible(globalVolumeSlider);
  addAndMakeVisible(volumeRange);
}
TitleBar::~TitleBar() {}

void TitleBar::paint(Graphics& g)
{
  auto area = getLocalBounds();
  auto bottomBorder = area.removeFromBottom(bottomBorderPx);

  g.setColour(AppStyle.darkGrey);
  g.fillRect(area);
  g.setColour(AppStyle.mediumGrey);
  g.fillRect(bottomBorder);
}

void TitleBar::resized()
{

  auto area = getLocalBounds();
  area.removeFromBottom(bottomBorderPx);
  auto buttonArea = area.removeFromLeft(componentWidth);

  componentPadding.subtractFrom(buttonArea);
  onOffButton.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(componentWidth);
  componentPadding.subtractFrom(buttonArea);
  globalVolumeSlider.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(2 * componentWidth);
  componentPadding.subtractFrom(buttonArea);
  volumeRange.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(componentWidth);
  componentPadding.subtractFrom(buttonArea);
  beatClockOnOffButton.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(componentWidth);
  componentPadding.subtractFrom(buttonArea);
  subdivisionSlider.setBounds(buttonArea);

  buttonArea = area.removeFromLeft(componentWidth);
  componentPadding.subtractFrom(buttonArea);
  midiInputOnOffButton.setBounds(buttonArea);
}

void TitleBar::updateComponents()
{
  // TODO only update GUI when needed
  onOffButton.updateComponent();
  beatClockOnOffButton.updateComponent();
  midiInputOnOffButton.updateComponent();
  subdivisionSlider.updateComponent();
  globalVolumeSlider.updateComponent();
  volumeRange.updateComponent();
}

/** On/Off Button */
OnOffButton::OnOffButton(WellsAudioProcessor& p) : TextButton("On/Off"), processor(p)
{
  onClick = [this]() { processor.midiGenerator->toggleOnOff(); };
}
OnOffButton::~OnOffButton() {}

void OnOffButton::updateComponent()
{
  setColour(
    TextButton::ColourIds::buttonColourId,
    processor.midiGenerator->getIsOn() ? AppStyle.buttonOnColour : AppStyle.buttonOffColour
  );
}

/** Beat Clock On/Off Button */
BeatClockOnOffButton::BeatClockOnOffButton(WellsAudioProcessor& p)
    : TextButton("Beat Clock"), processor(p)
{
  onClick = [this]() { processor.midiGenerator->toggleBeatClockIsOn(); };
}
BeatClockOnOffButton::~BeatClockOnOffButton() {}

void BeatClockOnOffButton::updateComponent()
{
  setColour(
    TextButton::ColourIds::buttonColourId,
    processor.midiGenerator->getBeatClockIsOn() ? AppStyle.buttonOnColour : AppStyle.buttonOffColour
  );
}

/** MIDI Input On/Off Button */
MidiInputOnOffButton::MidiInputOnOffButton(WellsAudioProcessor& p)
    : TextButton("MIDI Input"), processor(p)
{
  onClick = [this]() { processor.midiGenerator->toggleMidiInputIsOn(); };
}
MidiInputOnOffButton::~MidiInputOnOffButton() {}

void MidiInputOnOffButton::updateComponent()
{
  setColour(
    TextButton::ColourIds::buttonColourId,
    processor.midiGenerator->getMidiInputIsOn() ? AppStyle.buttonOnColour : AppStyle.buttonOffColour
  );
}

/*
 * Subdivision Slider
 */

SubdivisionSlider::SubdivisionSlider(WellsAudioProcessor& p) : Slider("Subdivision"), processor(p)
{
  setTooltip("subdivision");
  setSliderStyle(Slider::RotaryVerticalDrag);
  setRange(1, 256, 1);
  setTextBoxStyle(Slider::NoTextBox, false, 10, 0);
  setPopupDisplayEnabled(true, false, getParentComponent());
  onValueChange = [this]() { processor.midiGenerator->set_subdivision(getValue()); };
}
SubdivisionSlider::~SubdivisionSlider() {}

void SubdivisionSlider::updateComponent() { setValue(processor.midiGenerator->get_subdivision()); }

/*
 * Global Volume Slider
 */

GlobalVolumeSlider::GlobalVolumeSlider(WellsAudioProcessor& p) : Slider("Volume"), processor(p)
{
  setTooltip("global volume");
  setSliderStyle(Slider::RotaryVerticalDrag);
  setRange(0.0, 1.0, 0.00);
  setNumDecimalPlacesToDisplay(2);
  setTextBoxStyle(Slider::NoTextBox, false, 10, 0);
  setPopupDisplayEnabled(true, false, getParentComponent());
  onValueChange = [this]() { processor.midiGenerator->set_volume(getValue()); };
}
GlobalVolumeSlider::~GlobalVolumeSlider() {}

void GlobalVolumeSlider::updateComponent() { setValue(processor.midiGenerator->get_volume()); }

/*
 * Volume Range Slider
 */

VolumeRangeSlider::VolumeRangeSlider(WellsAudioProcessor& p)
    : Slider("MIDI Volume Range"), processor(p)
{
  setTooltip("volume range");
  setSliderStyle(Slider::TwoValueHorizontal);
  setRange(0, 1, 0.01);
  setTextBoxStyle(Slider::NoTextBox, false, 10, 0);
  setPopupDisplayEnabled(true, false, getParentComponent());
  onValueChange = [this]() {
    processor.midiGenerator->set_volume_clip(getMinValue(), getMaxValue());
  };
}
VolumeRangeSlider::~VolumeRangeSlider() {}

void VolumeRangeSlider::updateComponent()
{
  setMinValue(processor.midiGenerator->get_volume_clip_min());
  setMaxValue(processor.midiGenerator->get_volume_clip_max());
}
