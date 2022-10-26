/*
 * MidiInputBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiInputBar.hpp"
#include "Styles.hpp"

MidiInputBar::MidiInputBar(WellsAudioProcessor& p)
    : processor(p), onOffButton(p), inputWeightSlider(p)
{
  addAndMakeVisible(onOffButton);
  addAndMakeVisible(inputWeightSlider);
}

void MidiInputBar::paint(Graphics& g)
{
  auto area = getLocalBounds();
  AppStyle.blockPadding.subtractFrom(area);

  g.setColour(AppStyle.darkGrey);
  g.fillRoundedRectangle(area.toFloat(), 5.0);
  g.setColour(AppStyle.mediumGrey);
  g.drawRoundedRectangle(area.toFloat(), 5.0, 1.0);

  // make some space for the on/off button
  area.removeFromLeft(area.getHeight());

  auto rowLabelArea = area.removeFromLeft(AppStyle.rowLabelWidth);
  AppStyle.rowLabelPadding.subtractFrom(rowLabelArea);
  g.setFont(AppStyle.fontSizeMedium);
  g.setColour(AppStyle.lightGrey);
  g.drawText("MIDI Input", rowLabelArea, Justification::centredLeft, true);
}

void MidiInputBar::resized()
{
  auto area = getLocalBounds();
  AppStyle.blockPadding.subtractFrom(area);

  auto buttonArea = area.removeFromLeft(area.getHeight());
  AppStyle.inputModuleBarComponentPadding.subtractFrom(buttonArea);
  onOffButton.setBounds(buttonArea);

  area.removeFromLeft(AppStyle.rowLabelWidth);
  inputWeightSlider.setBounds(area.removeFromLeft(AppStyle.inputModuleBarRotarySliderWidth));
}

void MidiInputBar::updateComponents()
{
  onOffButton.updateComponent();
  inputWeightSlider.updateComponent();
}
