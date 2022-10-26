/*
 * BeatClockBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "BeatClockBar.hpp"

BeatClockBar::BeatClockBar(WellsAudioProcessor& p)
    : processor(p), onOffButton(p), inputWeightSlider(p), subdivisionSlider(p)
{
  addAndMakeVisible(onOffButton);
  addAndMakeVisible(inputWeightSlider);
  addAndMakeVisible(subdivisionSlider);
}

void BeatClockBar::paint(Graphics& g)
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
  g.drawText("Beat Clock", rowLabelArea, Justification::centredLeft, true);
}

void BeatClockBar::resized()
{
  auto area = getLocalBounds();
  AppStyle.blockPadding.subtractFrom(area);

  auto buttonArea = area.removeFromLeft(area.getHeight());
  AppStyle.inputModuleBarComponentPadding.subtractFrom(buttonArea);
  onOffButton.setBounds(buttonArea);

  area.removeFromLeft(AppStyle.rowLabelWidth);
  inputWeightSlider.setBounds(area.removeFromLeft(AppStyle.inputModuleBarRotarySliderWidth));
  subdivisionSlider.setBounds(area.removeFromLeft(AppStyle.inputModuleBarRotarySliderWidth));
}

void BeatClockBar::updateComponents()
{
  onOffButton.updateComponent();
  inputWeightSlider.updateComponent();
  subdivisionSlider.updateComponent();
}
